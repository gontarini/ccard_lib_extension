##Install
To install extensions you need to have ccard-lib installed -> https://github.com/chaoslawful/ccard-lib/
In the attached link it is explained what steps you need to follow in order to complete this process.

To build extensions, just type execute bash script:
- bash run.sh

In the source files you can find 3 php test scripts:
- test.php: presents proper way of using class without serializing objects
-test_serialized.php: presents proper way of using class with serializing objects
-bad_test.php: presents incorrect way of using class. Find prompt logs in order to see bad bitmap lengths passed as well as merging objects which were created in a different ways (with and without serialized object)


