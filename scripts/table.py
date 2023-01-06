#!/usr/bin/env python3

import os
import numpy as np


def get_path(
	prefix: str,
	n_bits: int,
	type_saved: str,
	compression: float):

	name = '{}_{}_{}_{:.1f}'.format(
		prefix,
		n_bits,
		type_saved,
		compression)

	if name[-1] == '0':
		name = name[0:-2]

	return os.path.join('test', name + '.jxl')


def main():
	bits = np.arange(2, 9, 1)
	compression = np.arange(0.1, 3.0, 0.1)

	with open('sw_lossy_quantization_jxl_float.csv', 'w') as f:
		f.write(',')
		for c in compression:
			f.write('{:.1f},'.format(c))
		f.write('\n')

		for n_bits in bits:
			f.write('{},'.format(n_bits))
			for c in compression:
				path = get_path(
					'sw_lossy_quantization_jxl',
					n_bits,
					'float',
					c)
				f.write('{},'.format(os.path.getsize(path)))
			f.write('\n')


if __name__ == '__main__':
	main()
