from PIL import Image
import numpy as np
fname = 'skybox.png'

im = np.asarray(Image.open(fname))
print(im.shape)
width = im.shape[0]//3
height = im.shape[1]//4

splits = {
    'top': (slice(0*width, 1*height), slice(1*width, 2*height)),
    'bottom': (slice(2*width, 3*height), slice(1*width, 2*height)),
    'left': (slice(1*width, 2*height), slice(0*width, 1*height)),
    'front': (slice(1*width, 2*height), slice(1*width, 2*height)),
    'right': (slice(1*width, 2*height), slice(2*width, 3*height)),
    'back': (slice(1*width, 2*height), slice(3*width, 4*height)),
}
for k, v in splits.items():
    print(k, v)
    Image.fromarray(im[v]).save(k + '.png')

