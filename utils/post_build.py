# to be called after building the solution.

from distutils.dir_util import copy_tree
import os

if __name__ == '__main__':
	src = 'assets'
	modes = ['Debug']
	projects = ['eclipse', 'example_app']
	for m in modes:
		for p in projects:
			copy_tree(src, os.path.join('bin', m, p, 'assets'))