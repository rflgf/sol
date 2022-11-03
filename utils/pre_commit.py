import functools
import logging
import os

def clang_format(project_dirs : list):
	or_fun = lambda x, y: x or y
	dirs_to_format = set()
	file_extensions = ['c', 'cpp', 'h', 'hpp', 'shader']
	ignored_dirs = ["vendor", "assets"]
	ignored_dirs_exceptions = [os.path.join("assets", "shaders")]

	for proj_dir in project_dirs:
		for dir, _, files_with_extensions in os.walk(proj_dir):
			should_format = True
			for ign_dir in ignored_dirs:
				if ign_dir in dir and not functools.reduce(
					or_fun,
					[(x in dir) for x in ignored_dirs_exceptions]):
					should_format = False
					break
			if not should_format:
				continue
			for fe in file_extensions:
				for fwe in files_with_extensions:
					if fwe.endswith(fe):			
						dirs_to_format.add(os.path.join(dir, f'*.{fe}'))
	os.system("clang-format -i " + ' '.join(dirs_to_format) +' --Werror')

if __name__ == "__main__":
	logging.basicConfig(level=logging.INFO)

	projects = ["sol", "eclipse", "example_app"]
	clang_format(projects)