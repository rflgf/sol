# written with SDL in mind, but can be tweaked to work
# with other libraries, I guess. basically aliases SDL key
# codes to names of my liking. :D

import os
	
if __name__ == "__main__":
	out_lines = []

	src_path = os.path.join('sol', 'vendor', 'SDL', 'include', 'SDL_keycode.h')
	out_path = 'out'

	with open(src_path, 'r') as src:
		for line in src.readlines():
			if line == '\n':
				out_lines.append(line)
				continue

			if '=' in line:
				# `SDL_NAME = RIGHT_HAND_SIDE`
				# will look like
				# `SOL_NAME = SDL_NAME`
				tokens = line.strip().split(' ')
				SDL_NAME = tokens[0]
				NAME = tokens[0][5:] # strips KMOD_ and SDLK
				out_lines.append('SOL_' + NAME + ' = ' + SDL_NAME + ',\n')

	with open(out_path, 'w') as out:
		for line in out_lines:
			out.write(line)