# -*- coding: utf-8 -*-
"""
@author: allexandre87
"""

import numpy as np
import matplotlib.pyplot as plt
from natsort import natsorted
from optparse import OptionParser

parser = OptionParser(usage       = 'python BuildGraph.py -i <filename input>  -o <filename output> -n <n_proc>',
                      prog        = 'BuildGraphs',
                      description = 'This program builds images using output .dat files from WaveSim.\n'
                                    'Must be executed in the directory where the FRAMES folder is.')

parser.add_option('-i', '--inp',
                  action='store', type = 'string',
                  help = 'Name of input files without extension.')

parser.add_option('-o', '--out',
                  action='store', type = 'string',
                  help = 'Name of output files without extension.')

parser.add_option('-n', '--nproc',
                  action='store', type = 'string',
                  help = 'Number of processors to use.')

options, arguments = parser.parse_args()

def BuildGraph(f, frames, files, fout):
    
    file  = files[f]
    frame = frames[f]
    
    data = np.loadtxt('./FRAMES/'+file, delimiter = ',')
    
    fig, ax = plt.subplots(figsize = (9, 9))
    ax.imshow(data.T, origin = 'lower',
              cmap = 'twilight',
              vmin = -0.9, vmax = 0.9,
              extent = [-1, 1, -1, 1])
    ax.axis('off')
    fig.tight_layout()
    fig.savefig(f'./FRAMES/{fout}_{frame+1}.png')
    plt.close()


if __name__ == '__main__':

    import os
    import multiprocessing as mp
    from functools import partial
    
    FILES = natsorted([_ for _ in os.listdir('./FRAMES/') if _.startswith(f'{options.inp}') and _.endswith('.dat')])
    
    plt.ioff()
    
    chunksize = 24
    
    maxiter = int(np.ceil(len(FILES)/chunksize))
    
    with mp.Pool(int(options.nproc)) as pool:    
        
        for iteration in range(maxiter):
            
            if (iteration+1)*chunksize < len(FILES):
                beg = iteration*chunksize
                end = (iteration+1)*chunksize
            else:
                beg = iteration*chunksize
                end = len(FILES)
            
            segment = FILES[beg:end]
            frames  = np.arange(len(segment))
            fnums   = np.arange(beg, end, 1)
            
            run = partial(BuildGraph,
                          frames = fnums,
                          files  = segment,
                          fout   = options.out)
            
            pool.map(run, frames)
            
            percent = int(100*(iteration+1)/maxiter)
            
            print(f'Done {percent} %', end = '\r')
            
    pool.close()
    pool.join()
