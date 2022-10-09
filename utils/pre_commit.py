import os
import logging

def clang_format(project_dirs : list):
	dirs_to_format = []
	ignored_dirs = ["vendor"]

	for proj_dir in project_dirs:
		for dir, _, _ in os.walk(proj_dir):
			should_format = True
			for ign_dir in ignored_dirs:
				if ign_dir in dir:
					should_format = False
					break
			if not should_format:
				continue
			dirs_to_format.append(os.path.join(dir, "*.cpp"))
			dirs_to_format.append(os.path.join(dir, "*.h"))

	# FIXME(rafael): this warns on directories with no *.h
	# or *.cpp files, maybe look into how to suppress that.
	logging.getLogger("sol").info('applying clang-format, "invalid'
		' argument" warnings can be ignored.')
	os.system("clang-format -i " + ' '.join(dirs_to_format))

if __name__ == "__main__":
	logging.basicConfig(level=logging.INFO)

	projects = ["sol", "eclipse", "example_app"]
	clang_format(projects)