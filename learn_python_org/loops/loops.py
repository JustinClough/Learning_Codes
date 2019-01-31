
primes = [2,3,5,7]
for prime in primes:
    print(prime)

for x in range(5):
    print( x)

for x in range(3,6):
    print( x)

for x in range(3,8,2):
    print( x)

count = 0
while count <5:
    print(count)
    count += 1

count = 0
while True:
    count += 1
    if count >= 5:
        break

for x in range(10):
    if x%2 ==0:
        continue
    print( x)

count = 0
while( count<5):
    print( count)
    count += 1
else:
    print( "count value reached %d" %count)

for i in range( 1,10):
    if( i%5 == 0):
        break
    print(i)
else:
    print("not printed becuase for loop is terminated")
    print("because of break not in `false` condition")

for i in range( 1,10):
    print(i)
else:
    print("IS printed becuase for loop is terminated")
    print("because of break not in `false` condition")
