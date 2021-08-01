# What's here?

Source code for bachelor's work about asymptotically efficient merging algorithms.

There are implementations for hwang-lin algorithm, manilla-ukkonen algorithm, symvonis algorithm (only case with enough distinct elements), geffert-katajainen-pasanen unstable algoritm.

Also there is tester for single-thread merging algorithms for two arrays, which can test algorithm for stability (in the sense of merging), quantity of element comparisons and cpu time.
This tester can be used not only for implemented here algorithms, but for any algorithm, implemented in special way (it must be template-based and have the same signature as implemented here algorithms).
