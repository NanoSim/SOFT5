# Use Case 4. Providing input to Phenom

This use case demonstrates how to use Porto to generate input to the MATLAB program Phenom. The code for this use case can be found in the `porto/src/phenom` folder of the Porto repository.

_TODO: We need some description of Phenom_

Since MATLAB does not presently directly communicate with Porto, we choose to generate a MATLAB function that will return the full set of input parameters needed to run the Phenom code.

This is done by:
* Retrieving an `phenom-input` entity from the database by referring to its uuid
* Populating a MATLAB function template with the contents of the `phenom-input` entity
* Writing the MATLAB function to a `.m` file that can be evaluated

This entire step is done by running the `make-phenom-input.js` script: 

```bash
$ ./make-phenom-input.js a6a71841-139a-4310-a9e6-ef7a6f161a6f > phenom-input.m
```