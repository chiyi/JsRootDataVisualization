#!/bin/python
import sys
import pandas
import numpy
import datetime

import ROOT
from ROOT import TFile, TCanvas, TH1D, THStack, TLegend, TAxis, TColor

def MakePlot(args):
    outdir = args[1]
    inpfile = args[2]
    df = pandas.read_csv(inpfile)
    df = df[df['Year']>1825]    # underflow for plots
    col_axis = ['Entity', 'Code']
    col_time = ['Year']
    col_sum = 'Sum(TWh)'
    col_values = numpy.setxor1d(df.columns, col_axis + col_time).tolist()
    df[col_sum] = df[col_values].sum(axis=1)
    col_values += [col_sum]

    t_minmax = df[col_time].min(), df[col_time].max()
    bin_edges = gen_edges(t_minmax)

    dict_h = dict()
    nbins = len(bin_edges)-1
    dt = 0.05
    dict_color = {
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
    for ith, icol in enumerate(col_values):
        hname = icol.replace('(TWh, substituted energy)', '').replace('(TWh)','').replace(' ','')
        print('hname=', hname)
        dict_h[hname] = TH1D(hname, icol, nbins, bin_edges)
        dict_h[hname].GetXaxis().SetTimeDisplay(1)
        dict_h[hname].GetXaxis().SetTimeFormat('%Y')
        dict_h[hname].GetXaxis().SetTimeOffset(0)
        hcolor = dict_color[hname] if hname in dict_color.keys() else 1
        dict_h[hname].SetLineColor(hcolor)
        dict_h[hname].SetFillColor(hcolor if hcolor!=1 else 0)
        dict_h[hname].SetFillStyle(1001)
        dict_h[hname].SetXTitle('Year')
        dict_h[hname].SetYTitle('consumed energy(TWh)')

   
        tmpdf = df[col_time + [icol]]
        for row_i in tmpdf.iterrows():
            val = row_i[1]
            val_t = get_ts(val[col_time]) + dt
            val_y = val[icol]
            ibin = dict_h[hname].Fill(val_t, val_y)
            dict_h[hname].SetBinError(ibin, 0.)   # no uncertainty values

        dict_h[hname].SaveAs(outdir+'/h_'+hname+'.json')

    sorted_hnames = sorted(map(lambda hname: (dict_h[hname].Integral(), hname) , dict_h.keys()))
    leg = TLegend(0.01, 0.81, 0.99, 0.93)
    leg.SetNColumns(6)
    hs_energy = THStack('hs_energy', 'Global primary energy consumption stacked by source')
    for (val_i, hname) in sorted_hnames:
        if hname=='Sum':
            continue
        hs_energy.Add(dict_h[hname])
        leg.AddEntry(dict_h[hname], hname, 'F')

    leg.AddEntry(dict_h['Sum'], 'Sum', 'L')

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
    leg.Draw('SAME')
    c_stack.SetLogy()
    c_stack.SaveAs(outdir+'/c_stacked_energy.json')


    fw = TFile(outdir+'/Energy_Consumption.root', "RECREATE")
    fw.cd()
    for hname in dict_h:
        dict_h[hname].Write()
    hs_energy.Write()
    c_stack.Write()
    fw.Close()

    return


def get_ts(ts_data):
    year = int(ts_data['Year']) if 'Year' in ts_data else 1970
    month = int(ts_data['Month']) if 'Month' in ts_data else 1
    day = int(ts_data['Day']) if 'Day' in ts_data else 1
    hour = int(ts_data['Hour']) if 'Hour' in ts_data else 0
    minute = int(ts_data['Minute']) if 'Minute' in ts_data else 0
    second = int(ts_data['Second']) if 'Second' in ts_data else 0
    zone_off = int(ts_data['Zone']*3600) if 'Zone' in ts_data else 0
    dt = datetime.datetime(
        year, month, day,
        hour, minute, second,
        tzinfo=datetime.timezone(datetime.timedelta(seconds=zone_off))
    )
    ts = int(dt.timestamp())
    return ts

def gen_edges(t_minmax, mode='Year'):
    res = []
    m_Y = 'Year'
    m_M = 'Month'
    m_D = 'Day'
    m_h = 'Hour'
    m_m = 'Minute'
    m_s = 'Second'
    ts_min = get_ts(t_minmax[0])
    ts_max = get_ts(t_minmax[1])

    if mode in [m_Y, m_M]:
        for iy in range(t_minmax[0][m_Y], t_minmax[1][m_Y]+1):
            (M_min, M_max) = (1, 13) if mode==m_M else (1, 2)
            for im in range(M_min, M_max):
                res.append(get_ts({m_Y:iy, m_M:im}))
        res.append(get_ts({m_Y:t_minmax[1][m_Y]+1}))
    elif mode in [m_D, m_h, m_m, m_s]:
        inc = {m_D:86400, m_h:3600, m_m:60, m_s:1}[mode]
        res = list(range(int(ts_min), int(ts_max)+inc*2, inc))
    else:
        res = [0]
    return numpy.array(res, numpy.double)
    

print('sys.argv =', sys.argv)
MakePlot(sys.argv)


