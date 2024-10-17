
#These functions calculate the number of ways n buckets can be occupied by up to S items such that no single bucket contains more than M items.
def countofwaysinit(n, S):
  l = list()
  dplol = list()
  for i in range(S+1):
    l.append(-1)
  for i in range(n+1):
    dplol.append(l.copy())
  return dplol;

def countofways(i, j, M, dplol):
  if (i <= 0):
    return 0
  if (j < 0):
    return 0
  if (j == 0):
    return 1
  if (i == 1):
    return 1+max(0,min(j,M))
  if (dplol[i][j] >= 0):
    #print(i,j,dplol[i][j])
    return dplol[i][j]
  res = 0
  kmax = min(M,j)
  for k in range(kmax+1):
    res += countofways(i-1, j-k, M, dplol)
  dplol[i][j] = res
  #print(i,j,res)
  return res
  
def gethashpopcount(hashstring):
  hashstring = hashstring.lower()
  hexnibbles = {"0":0, "1":1, "2":1, "3":2, "4":1, "5":2, "6":2, "7":3, "8":1, "9":2, "a":2, "b":3, "c":2, "d":3, "e":3, "f":4}
  res = 0
  for c in hashstring:
    res += hexnibbles[c]
  return res
  
def gethashbytesum(hashstring):
  hashstring = hashstring.lower()
  hexnibbles = {"0":0, "1":1, "2":2, "3":3, "4":4, "5":5, "6":6, "7":7, "8":8, "9":9, "a":10, "b":11, "c":12, "d":13, "e":14, "f":15}
  res = 0
  factor = 16
  for c in hashstring:
    res += factor*hexnibbles[c]
    factor = 17 - factor
  return res

def gethashvalue(hashstring):
  hashstring = hashstring.lower()
  hexnibbles = {"0":0, "1":1, "2":2, "3":3, "4":4, "5":5, "6":6, "7":7, "8":8, "9":9, "a":10, "b":11, "c":12, "d":13, "e":14, "f":15}
  res = 0
  for c in hashstring:
    res *= 16
    res += hexnibbles[c]
  return res
  
#This function prints statistics of a hash input as a hex string
def printhashstats(hashstring):
  hashstringlen = len(hashstring)
  hashvalue = gethashvalue(hashstring)
  n = 4*hashstringlen
  twopown = 2**n
  print("Chance of lower value = 1 :", twopown/hashvalue)
  print("Chance of higher value = 1 :", twopown/(twopown - hashvalue - 1))
  hashpopcount = gethashpopcount(hashstring)
  print("Popcount(", hashstring, ") = ", hashpopcount)
  S = hashpopcount - 1
  dplol = countofwaysinit(n, S)
  print("Chance of lower popcount = 1 :", twopown/countofways(n, S, 1, dplol))
  del dplol
  S = n - hashpopcount - 1
  dplol = countofwaysinit(n, S)
  print("Chance of higher popcount = 1 :", twopown/countofways(n, S, 1, dplol))
  del dplol
  hashbytesum = gethashbytesum(hashstring)
  print("Bytesum(", hashstring, ") = ", hashbytesum)
  n = hashstringlen//2
  S = hashbytesum - 1
  dplol = countofwaysinit(n, S)
  print("Chance of lower bytesum = 1 :", (256**n)/countofways(n, S, 255, dplol))
  del dplol
  S = n*255 - hashbytesum - 1
  dplol = countofwaysinit(n, S)
  print("Chance of higher bytesum = 1 :", (256**n)/countofways(n, S, 255, dplol))
  del dplol
  return
  
printhashstats("110d0316000b020d09010205002a0a00")
#n = 160
#S = 30
#M = 1
#dplol = countofwaysinit(n, S)
#ans = countofways(n, S, M, dplol)
#print(ans)
