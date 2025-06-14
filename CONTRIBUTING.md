# Contributing
## Setup
To set up the necessary working environment, simply run:

```sh
$ ./deploy_dev.sh
```

That should add all the necessary Git hooks.

## Tools
To build the application with recommended flags, run `./build.sh`.

To build the unit tests, run `./build_tests.sh` and execute `./build/test`.
No output signifies passing all tests.

Make sure to run these scripts from time to time.
However, the included Git hooks should automatically:
 - `pre-commit`: Check that the program builds and is properly formatted;
 - `commit-msg`: Check that the supplied commit message follows [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/);
 - `pre-push`: Check that the program passes all tests.
