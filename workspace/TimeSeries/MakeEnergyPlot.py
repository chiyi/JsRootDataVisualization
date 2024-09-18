#!/bin/python
import sys
import pandas
import numpy
import datetime
import enum

import ROOT
from ROOT import TFile, TCanvas, TH1D, THStack, TLegend, TAxis, TColor

class TimeMode(enum.Enum):
    UNKNOWN = 0
    YEAR = 1
    MONTH = 2
    DAY = 3
    HOUR = 4
    MINUTE = 5
    SECOND = 6

    # Dictionary for increments
    increments = {DAY: 86400, HOUR: 3600, MINUTE: 60, SECOND: 1}

    def __lt__(self, other):
      if self.__class__ is other.__class__:
        return self.value < other.value
      return NotImplemented

def prepare_df(file_path: str, cols_time: list[str], start_year: int=1825) \
    -> tuple[pandas.DataFrame, list[str]]:
    """
    Prepare a DataFrame from a CSV file by filtering and summarizing data.

    This function reads a CSV file, filters out entries before a specified year,
    and adds a new column 'Sum(TWh)' which is the sum of all value columns
    excluding 'Entity', 'Code', and time columns.

    :param file_path: Path to the CSV file
    :param cols_time: List of column names representing time data
    :param start_year: The threshold year for filtering (default is 1825)
    :return: Tuple of (DataFrame with new sum column, List of value column names including the sum)
    """
    cols_axis = ['Entity', 'Code']
    col_sum = 'Sum(TWh)'

    df = pandas.read_csv(file_path)
    df = df[df['Year'] > start_year]
    col_values = numpy.setdiff1d(df.columns, cols_axis + cols_time).tolist()
    df[col_sum] = df[col_values].sum(axis=1)
    col_values.append(col_sum)
    return df, col_values

def generate_bin_edges(df, cols_time, mode=TimeMode.YEAR) -> numpy.ndarray[numpy.double]:
    """
    Generate bin edges based on time data in the DataFrame.

    :param df: DataFrame containing time data
    :param cols_time: List of column names representing time
    :param mode: Time granularity for bin edges (TimeMode enum)
    :return: Numpy array of bin edges as timestamps
    """
    df_sorted = df.sort_values(by=cols_time)
    t_min, t_max = df_sorted.iloc[[0, -1]][cols_time].to_dict('records')
    ts_min, ts_max = get_timestamp(t_min), get_timestamp(t_max)
    if mode < TimeMode.DAY:
        res = []
        for year in range(t_min['Year'], t_max['Year'] + 1):
            for month in range(1, 13 if mode == TimeMode.MONTH else 2):
                res.append(get_timestamp({'Year': year, 'Month': month}))
        res.append(get_timestamp({'Year': t_max['Year'] + 1}))  # Add one year after max year
    else:
        inc = TimeMode.increments[mode]
        res = list(range(int(ts_min), int(ts_max) + inc * 2, inc))
    return numpy.array(res, dtype=numpy.double)

def get_timestamp(ts_data: dict[str, int]) -> int:
    """
    Convert a dictionary of date components to a Unix timestamp.

    This function assumes 'Year' is always present and uses default values
    for missing components, with UTC as the default timezone. If any component
    cannot be converted to an integer, the function will fail.

    :param ts_data: Dictionary containing date components
    :return: Unix timestamp as an integer
    """
    year = int(ts_data.get('Year', 1970))
    month = int(ts_data.get('Month', 1))
    day = int(ts_data.get('Day', 1))
    hour = int(ts_data.get('Hour', 0))
    minute = int(ts_data.get('Minute', 0))
    second = int(ts_data.get('Second', 0))
    zone_off = int(ts_data.get('Zone', 0) * 3600)  # Convert hours to seconds
    dt = datetime.datetime(
        year, month, day, hour, minute, second,
        tzinfo=datetime.timezone(datetime.timedelta(seconds=zone_off))
    )
    ts = int(dt.timestamp())
    return ts

def set_dictcolor() -> dict[str, int]:
    """
    Define a dictionary mapping energy sources to color codes.

    This function returns a dictionary where keys are energy source names
    and values are corresponding color codes.

    :return: A dictionary with energy sources as keys and color codes as values.
    """
    return {
        'Coal': 45,
        'Oil': 46,
        'Gas': 618,
        'Nuclear': 30,
        'Hydropower': 38,
        'Wind': 603,
        'Solar': 623,
        'Traditionalbiomass': 44,
        'Biofuels': 41,
        'Otherrenewables': 32
    }

def init_histogram(hname: str, title: str, nbins: int, bin_edges: list[float], dict_color: dict[str, int]) -> TH1D:
    """
    Initialize a TH1D histogram with custom settings for energy consumption data.

    This function sets up a histogram with time-based bin edges, custom colors,
    and labels suitable for displaying energy consumption over time.

    :param hname: Name of the histogram, used for identification and color mapping
    :param title: Title of the histogram
    :param nbins: Number of bins in the histogram
    :param bin_edges: List of bin edges, typically timestamps for time-based data
    :param dict_color: Dictionary mapping histogram names to color codes
    :return: A configured TH1D histogram object
    """
    hist = TH1D(hname, title, nbins, numpy.array(bin_edges, numpy.double))
    hist.GetXaxis().SetTimeDisplay(1)
    hist.GetXaxis().SetTimeFormat('%Y')
    hist.GetXaxis().SetTimeOffset(0)
    color = dict_color.get(hname, 1)
    hist.SetLineColor(color)
    hist.SetFillColor(color if color != 1 else 0)
    hist.SetFillStyle(1001)
    hist.SetXTitle('Year')
    hist.SetYTitle('Consumed Energy (TWh)')
    return hist

def fill_histogram(hist: TH1D, df: pandas.DataFrame, column: str, columns_time: list = ['Year'], dt: float = 0) -> None:
    """
    Fill a TH1D histogram with data from a DataFrame.

    :param hist: TH1D histogram to fill
    :param df: DataFrame containing data
    :param column: Column name for values
    :param columns_time: List of column names for time data
    :param dt: Time offset in seconds
    """
    for _, row in df[columns_time + [column]].iterrows():
        bin_i = hist.Fill(get_timestamp(row[columns_time]) + dt, row[column])
        hist.SetBinError(bin_i, 0)

def gen_hstack(dict_h: dict[str, TH1D]) -> tuple[THStack, TLegend]:
    """
    Generate a THStack from a dictionary of histograms and create a legend.

    This function sorts histograms by their integral for stacking, creates a THStack,
    and sets up a legend with entries for each histogram.

    :param dict_h: Dictionary where keys are histogram names and values are TH1D objects
    :return: A tuple containing the THStack and TLegend objects
    """
    # Sort histograms by integral for stacking
    sorted_hnames = sorted(dict_h.keys(), key=lambda hname: dict_h[hname].Integral())
    hs_energy = THStack('hs_energy', 'Global primary energy consumption stacked by source')
    legend = TLegend(0.01, 0.81, 0.99, 0.93)
    legend.SetNColumns(6)
    for hname in sorted_hnames:
        if hname=='Sum':
            continue
        hs_energy.Add(dict_h[hname])
        legend.AddEntry(dict_h[hname], hname, 'F')
    legend.AddEntry(dict_h['Sum'], 'Sum', 'L')
    return hs_energy, legend

def save_plots(outdir: str, dict_h: dict[str, TH1D], hs_energy: THStack, legend: TLegend) -> None:
    """
    Save histograms and stacked plot to various file formats.

    This function creates a canvas, draws the stacked histogram, and saves it along with
    individual histograms in JSON and ROOT formats.

    :param outdir: Directory path where files will be saved
    :param dict_h: Dictionary of histograms to save
    :param hs_energy: THStack object containing stacked histograms
    :param legend: TLegend object for the plot
    """
    c_stack = TCanvas('c_stack', 'canvas stacked energy plot', 1800, 800)
    c_stack.cd()
    c_stack.SetGridy(1)
    c_stack.SetTopMargin(0.2)
    hs_energy.Draw()
    hs_energy.GetXaxis().SetTitle('Year')
    hs_energy.GetXaxis().SetTimeDisplay(1)
    hs_energy.GetXaxis().SetTimeFormat('%Y')
    hs_energy.GetXaxis().SetTimeOffset(0)
    hs_energy.GetYaxis().SetTitle('consumed energy(TWh)')
    dict_h['Sum'].Draw('SAME')
    legend.Draw('SAME')
    c_stack.SetLogy()
    c_stack.SaveAs(f"{outdir}/c_stacked_energy.json")

    for hname in dict_h.keys():
        dict_h[hname].SaveAs(f"{outdir}/h_{hname}.json")

    with TFile(f"{outdir}/Energy_Consumption.root", "RECREATE") as fw:
        fw.cd()
        for hname in dict_h.keys():
            dict_h[hname].Write()
        hs_energy.Write()
        fw.WriteObject(legend, "legend")
        c_stack.Write()




def MakeEnergyPlot(args):
    """
    Generate and save energy consumption plots from input data.

    :param args: Command-line arguments including output directory and input file
    """
    if len(args) < 3:
        print("Usage: ./MakeEnergyPlot.py <output_directory> <input_file>")
        sys.exit(1)

    outdir, inpfile = args[1], args[2]
    cols_time = ['Year']

    df, col_values = prepare_df(inpfile, cols_time)
    bin_edges = generate_bin_edges(df, cols_time)

    dict_h = dict()
    nbins = len(bin_edges) - 1
    dt = 0.05
    dict_color = set_dictcolor()
    for ith, icol in enumerate(col_values):
        hname = icol.replace('(TWh, substituted energy)', '').replace('(TWh)','').replace(' ','')
        print('hname=', hname)
        dict_h[hname] = init_histogram(hname, icol, nbins, bin_edges, dict_color)
        fill_histogram(dict_h[hname], df, icol, cols_time, dt)
    hs_energy, legend = gen_hstack(dict_h)
    save_plots(outdir, dict_h, hs_energy, legend)

print('sys.argv =', sys.argv)
MakeEnergyPlot(sys.argv)
