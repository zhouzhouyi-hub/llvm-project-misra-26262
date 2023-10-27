.. title:: clang-tidy - misrac-rule6_2

misrac-rule6_2
==============

MISRA C rule 6.2 require Single-bit named bit fields shall not be of a signed type:

.. code-block:: c++

  struct s {
    int b1:1;
  };


