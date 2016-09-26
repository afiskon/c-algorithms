# c-algorithms

Collection of BSD/MIT-licensed algorithms implementations in pure C. High test coverage.

## Usage

How to build:

```
mkdir build
cd build
cmake ..
make -j4
make test
```

How to make a coverage report:

```
sudo apt-get install lcov

mkdir build
cd build
../cov.sh
```

## Index

* Data Structures (src/struct/)
	* Single- and double-linked lists (ilist.c)
	* Red-black trees (rbtree.c)
	* Hash tables (htable.c)
* Crytography (src/crypto/)
	* MD5 Hashing (crypto/md5.c)
	* More will be added soon!

