# Quadratic-Seive-Demo
simpe demo for quadratic seive factoring algorithm

<h2> How to build</h2>

  ```sh
  // instill boost lib first
  sudo apt update
  sudo apt install libboost-all-dev
  ```
  ```sh
  g++ quadratic_seive.cpp -o quadratic_seive
  ./quadratic_seive
  ```
<h2> Sample output </h2>

  ```sh
  Load 78523 primes...
  
  N=209
  --------------------
  B=16
  Found 7 smooth number.
  --------------------
  209 = 11 * 19
  Execution time: 0 seconds
  
  
  N=3168656483
  --------------------
  B=587
  Found 37 smooth number.
  B=1174
  Found 110 smooth number.
  --------------------
  3168656483 = 61819 * 51257
  Execution time: 0 seconds
  
  
  N=939452277403
  --------------------
  B=1316
  Found 88 smooth number.
  --------------------
  939452277403 = 999863 * 939581
  Execution time: 0 seconds
  
  
  N=100001880003211
  --------------------
  B=4006
  Found 460 smooth number.
  --------------------
  100001880003211 = 10000169 * 10000019
  Execution time: 0 seconds
  
  
  N=48932168077212074407
  --------------------
  B=21169
  Found 968 smooth number.
  --------------------
  48932168077212074407 = 8292675673 * 5900648959
  Execution time: 6 seconds
  
  
  N=5170953267000447024451
  --------------------
  B=39167
  Found 1160 smooth number.
  B=78334
  Found 3374 smooth number.
  --------------------
  5170953267000447024451 = 57439585883 * 90024208697
  Execution time: 110 seconds
  
  
  N=29996224771700583503795323
  --------------------
  B=125365
  Found 3024 smooth number.
  B=250730
  Found 8364 smooth number.
  --------------------
  29996224771700583503795323 = 29996224275833 * 1000000016531
  Execution time: 1000 seconds
  ```

