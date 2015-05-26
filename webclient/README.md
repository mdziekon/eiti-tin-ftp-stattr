WebClient
=================

# Client

## Prerequisites

### System packages
```bash
$ sudo apt-get install nodejs nodejs-legacy npm
```

### Fetch all dependencies
```bash
$ cd client
$ npm install
```

_(In case of EACCES problems)_
```bash
$ sudo chown -R $(whoami) ~/.npm
```

## How to run/compile it?

### Local run, without compilation

_(All files will be available in ``client/app`` folder)_
```bash
$ cd client
$ grunt serve
```

### Production compilation

_(All files will be compiled in ``client/dist`` folder)_
```bash
$ cd client
$ grunt build
```
