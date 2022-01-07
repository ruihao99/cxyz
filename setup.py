from setuptools import setup
from setuptools.extension import Extension
from Cython.Build import cythonize
import numpy

setup(
    name="cxyz",
    version="0.0.1",
    author="Ruihao Bi",
    author_email="brh6@qq.com",
    description="Fast xyz trajectory reader",
    long_description_content_type="text/markdown",
    packages=setuptools.find_packages(),

    ext_modules = cythonize([Extension("cxyz", ["cxyz.pyx"], 
            include_dirs=[numpy.get_include(), "./include"],
            language="c++"
            )]),

    classifiers=[
        "Programming Language :: Python :: 3",
    ],
    python_requires='>=3.6',
    install_requires=[
        "numpy",
        "cython"
  ]
)
