'''
Routines to perform different types of plots.
'''
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
import visualization as vis


def plot_hydro_vars(all_vars, which='duc'):
    fig, ax = plt.subplots(len(which), 1, figsize=(15,10))
    
    for i in range(len(which)):
        var = which[i]
        ax[i].plot(all_vars['x'], all_vars[var], linewidth=2)
        ax[i].set_ylabel(var, fontsize='large')
        
    ax[len(which) - 1].set_xlabel('radius', fontsize='large')
    
    return ax
    
    
def plot_velocity_data(upper, folder):
    dat = np.loadtxt(vis.full_path(upper, folder, 'velocity_history.txt'))
    
    fig, ax = plt.subplots(figsize=(10,6))
    ax.plot(dat[:,0], dat[:,1])
    return ax
    
    
def plot_shock_front(ax, vars=None, fname=None, axis=True):
    if vars:
        ax[0].plot(vars['x'], vars['d'], label=vis.extract_index_from_folder(fname))
        ax[1].plot(vars['x'], vars['u']/vars['c'], label=vis.extract_index_from_folder(fname))

        ax[0].set_xlim(0, 1.5)
        ax[0].legend()
        ax[1].set_xlim(-0.1, 1.1)
        ax[1].legend()

    # Ax1 is density vs position
    # Ax2 is v / c = M vs Position
    
    if axis:
        ax[0].set_xlabel(r"X")
        ax[0].set_ylabel(r"$\rho$")

        ax[1].set_xlabel(r"X")
        ax[1].set_ylabel(r"$M$")
    
    
def many_shock_front(num, files, path, gamma, end=0.3):
    skip = 0
    count = int(2 * len(files) / num)
    
    fig, ax = plt.subplots(1, 2, figsize=(20, 10))
    
    for key, item in files:
        if skip > 1:
            skip = (skip + 1) if skip <= count else 0
            continue
         
        sub = int(end * len(item))
        vars = vis.read_hydro_variables(vis.full_path(path, key, item[-sub]), gamma)

        plot_shock_front(ax, vars, key, axis=False)
        skip += 1
      
    plot_shock_front(ax, axis=True)
    
    return ax

    
def fit_breakout_velocity(impactors, breakout_v, deg=1):
    coeffs = np.polyfit(np.log(impactors), np.log(breakout_v), deg = deg)
    poly = np.poly1d(coeffs)
    yfit = lambda x: np.exp(poly(np.log(x)))
    
    return [poly, yfit]
    

def plot_breakout_velocity(folders, upper):
    impactors = vis.get_impactors_list(folders)
    breakout_v = vis.get_breakout_velocity(folders, upper)
    
    poly, yfit = fit_breakout_velocity(impactors, breakout_v)
    
    fig, ax = plt.subplots(figsize=(10,6))
    ax.loglog(impactors, breakout_v, 'go')
    ax.set(xlabel = "Impactor Size",
           ylabel = "Breakout velocity")
    
    ax.loglog(impactors, yfit(impactors), 'k', linewidth=2)
    
    return poly
    