### DBMS query algorithm implement

#### Environment
```
OS: Deepin 15.9.3 unstable
Kernel: x86_64 Linux 4.15.0-29 deepin-generic
GNU Make 4.2.1
g++ --version (Debian 7.3.0-19) 7.3.0
```

#### How to run

first, you need to create B plus tree index by running:
```bash
make index
```
then,
you can show  result of `select` operation by running:
```
make select
```
or show  result of `projection` operation by running:
```
make projection
```
or show  result of `join` operation by running:
```
make join
```

#### Hint
All sort operation is done by B plus tree