import os

def clang_format(project_dirs : list):
	dirs_to_format = []
	
	for proj_dir in project_dirs:
		for dir, _, files in os.walk(proj_dir):
			for filename in files:
				if filename.endswith(".cpp") or filename.endswith(".h"):
					dirs_to_format.append(os.path.join(dir, "*"))
					break
	
	os.system("clang-format -i " + ' '.join(dirs_to_format))

if __name__ == "__main__":
	projects = ["sol", "eclipse", "example_app"]

	clang_format(projects)