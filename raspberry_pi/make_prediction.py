#!/usr/bin/python3

import keras
import json
import numpy
import keras
import pickle
import surk

stuff_to_load = json.load(open('ttracker.model', 'r'))
weights = [numpy.array(w) for w in stuff_to_load['weight']]
arch = stuff_to_load['arch']
model = keras.models.model_from_json(arch)
model.set_weights(weights)
model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
s = surk.Surk()


X = numpy.array([s.sample_one_second()]) 

print(X.shape)

print(model.predict(X))
#load a model from a file we are about to download from the fancy box