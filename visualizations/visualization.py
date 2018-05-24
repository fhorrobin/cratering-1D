'''
Routines to help with visualization.
'''
import h5py
import numpy as np
import re
import glob

def extract_index_from_filename(fname):
    """
    Get the number suffix from the filename
    """
    return int(re.search(r'_(\d+).', fname).group(1))


def extract_index_from_folder(folder):
    """
    Get the number suffix from a folder name
    """
    return float(re.search(r'_(\d+\.\d+)', folder).group(1))

    
def full_path(*args):
    """
    Args are strings. Join all of the strings to form a file path.
    If the string ends with `/` do not append another one otherwise
    append a `/` to the string. 
    """
    path = ""
    for item in args:
        if item[-1] == "/":
            path += item
        else:
            path += item + "/"
            
    return path[:-1]


def get_files(folder):
    """
    Get all the snapshots from a folder as sorted list.
    """
    files = glob.glob(full_path(folder, "/snapshot_*.h5"))
    files = [f[len(folder) + 1:] for f in files]
    return sorted(files, key=extract_index_from_filename)


def get_folders(upper):
    """
    Get all the results folders for a value of gamma.
    """
    folders = glob.glob(full_path(upper, "res_*"))
    folders = [f[len(upper) + 1:] for f in folders]
    return sorted(folders, key=extract_index_from_folder)

def sample_velocity(fname, position):
    """
    Sample (interpolate) the velocity at x location `position'
    for h5 file `fname'.
    """
    with h5py.File(fname, 'r') as f:
        u_list = np.array(f['x_velocity'])
        x_list = np.array(f['grid'])
        
    return np.interp(position, x_list, u_list)


def read_hydro_variables(fname, gamma):
    '''
    Read hdf5 file and return np arrays with the
    desired hydro variables (d, u, x, p, c).
    '''
    with h5py.File(fname, 'r') as f:
        d_list = np.array(f['density'])
        u_list = np.array(f['x_velocity'])
        x_list = np.array(f['grid'])
        p_list = np.array(f['pressure'])
        
    c_list = np.sqrt(gamma * p_list / d_list)
    
    return {'d': d_list, 'u': u_list, 'x': x_list, 'p': p_list, 'c': c_list}


def file_list(folders, upper):
    all_files = []
    for f in folders:
        files = get_files(full_path(upper, f))
        all_files.append((f, files))
        
    return all_files


def get_impactors_list(folders):
    impactors = []

    for folder in folders:
        impactors.append(extract_index_from_folder(folder))
        
    return impactors
    
    
def get_breakout_velocity(folders, upper):
    breakout_v = []
    for folder in folders:
        breakout_v.append(np.max([sample_velocity(f, 0.99) for f in glob.glob(upper + "/" + folder + "/snapshot_*.h5")]))
        
    return breakout_v
