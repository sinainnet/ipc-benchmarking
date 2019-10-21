## How to install

### p1
````
$ LD_PRELOAD=/usr/local/lib/libxpmem.so ./xpmem_proc1
````

### p2
````
$ LD_PRELOAD=/usr/local/lib/libxpmem.so ./xpmem_proc2
````


## Results without semaphore

### 1-Gigabyte

time in nanoseconds:

- 581379066
- 586321675
- 583344418
- 581416783
- 582653025
- 583589487

### 2-Gigabyte

time in nanoseconds:

- 1.167612513
- 1.168786209
- 1.165896179
- 1.168013268
- 1.165170471
- 1.169532954


### 3-Gigabyte

time in nanoseconds:

- 1.756822524
- 1.751173598
- 1.748741100
- 1.751762570
- 1.746357980
- 1.747138490


### 4-Gigabyte

time in nanoseconds:

- 2.349303082
- 2.345735161
- 2.329898724
- 2.320331189
- 2.332429423
- 2.392663601



### 8-Gigabyte

time in nanoseconds:

- 4.650524226
- 4.659491382
- 4.678995038
- 4.668918348
- 4.682671836
- 4.661486714



## Results with semaphore

### 1-Gigabyte

time in nanosecond:

- 585105959
- 582921067
- 585180882
- 585560497
- 582873555
- 583817160


### 2-Gigabyte

time in nanosecond:

- 1.166622371
- 1.163743761
- 1.169896238
- 1.167371051
- 1.169321121
- 1.162907217


### 3-Gigabyte

time in nanoseconds:

- 1.758853642
- 1.751597541
- 1.753025669
- 1.757578798
- 1.747572392
- 1.746453239

### 4-Gigabyte

time in nanoseconds:

- 2.335050597
- 2.337399761
- 2.352049198
- 2.327910714
- 2.331979717
- 2.331918063

### 8-Gigabyte

time in nanoseconds:

- 4.671463245
- 4.696017850
- 4.653127581
- 4.667323344
- 4.656641233
- 4.633653682