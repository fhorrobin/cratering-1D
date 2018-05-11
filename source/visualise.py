import matplotlib
matplotlib.use('Agg')

def consolidate(fname):

    import h5py
    import numpy

    with h5py.File(fname,'r+') as f:
        return {field:numpy.array(f[field])
                for field in f}

def h5_to_png(fname):

    import matplotlib.pyplot as plt

    data = consolidate(fname)

    f, ax = plt.subplots(313)

    print ax

if __name__ == '__main__':

    h5_to_png('snapshot_0.h5')
