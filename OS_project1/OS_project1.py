#!/usr/bin/env python
# coding: utf-8

# In[1]:


import queue
import time
import multiprocessing 
import threading

'''
資訊三甲 10627149 許峯僑
OS Project1

Case :
1.Make N numbers with Bubble Sort and display the CPU execution time.

2.Cut N numbers into k parts, and make the Bubble Sort by K threads, 
  Merge Sort by k-1 threads and display the CPU execution time.
  
3.Cut N numbers into k parts, and make the Bubble Sort by K Processes,
  Merge Sort by k-1 Processes and display the CPU execution time.
  
4.Cut N numbers into k parts, and make the Bubble Sort on the K parts of data in a process,
  use the same process for Merge Sort and display the CPU execution time.

'''


def GetList(fileName, file):
    content = file.read() # read the rest of the data
    content_ls = content.split()# seprate the content by ' '
    int_con_ls = list(map(int, content_ls)) # convert all content from String type to Integer type
    return int_con_ls

    

def CutList(fileName, sepNum, f):
    int_con_ls = GetList(fileName, f)
    lenOfcont = len(int_con_ls)
    n = len(int_con_ls) // sepNum

    sep_ls = [int_con_ls[i:i+n] for i in range(0, n*sepNum, n)] # seprate the content_ls with n, which is k
    sep_ls[sepNum-1].extend(int_con_ls[n*sepNum:lenOfcont]) # concat the rest of the content_ls to the last list
    return sep_ls

#
#	Quick Sort
#
def QuickSort( A ):
	n = len( A )
	_QuickSort( A, 0, n - 1 )


def _QuickSort( A, p, r ):
	if p < r:
		q = Partition( A, p, r )
		_QuickSort( A, p, q - 1 )
		_QuickSort( A, q + 1, r )


def Partition( A, p, r ):
	x = A[r]
	i = p - 1
	for j in range( p, r ):
		if A[j] <= x:
			i += 1
			A[i], A[j] = A[j], A[i]
	A[i + 1], A[r] = A[r], A[i + 1]
	return i + 1



#
#	Bubble Sort
#    
    
def BubbleSort(flist, slist): # file list , sorted list

    lenOfList = len(flist)   # file list length 
    
    for i in range(lenOfList): # bubble sort
        for j in range(lenOfList-1-i):
            if flist[j] > flist[j+1]:
                flist[j], flist[j+1] = flist[j+1], flist[j] # swap
                
    slist.put(flist) # put the sorted file into slist 

#
#	Merge Sort
#
def mergeSort( A ):
	n = len( A )
	_MergeSort( A, 0, n - 1 )


def _MergeSort( A, p, r ):
	if p < r:
		q = ( p + r ) // 2
		_MergeSort( A, p, q )
		_MergeSort( A, q + 1, r )
		Merge( A, p, q, r )


def Merge( A, p, q, r ):
	n1 = q - p + 1
	n2 = r - q
	
	L = []
	R = []
	for i in range( n1 ):
		L.append( A[p + i] )
		
	for j in range( n2 ):
		R.append( A[q + j + 1] )
		
	i = j = 0
	for k in range( p, r + 1 ):
		if i < n1 and j < n2:
			if L[i] <= R[j]:
				A[k] = L[i]
				i += 1
			else:
				A[k] = R[j]
				j += 1
		elif i < n1 and j >= n2:
			A[k] = L[i]
			i += 1
		else:
			A[k] = R[j]
			j += 1
   
    
def MergeSort(llist, rlist, slist): # use leftlist and rightlist sort

    l, r = 0, 0
    lenOfllist = len(llist)
    lenOfrlist = len(rlist)
    items = []  # sorted list
    
    while l < lenOfllist and r < lenOfrlist: # if one of the list is empty then breaks out
        if llist[l] < rlist[r]:
            items.append(llist[l])
            l += 1
        else : # _ll[l] > _rl[r]
            items.append(rlist[r])
            r += 1

    # after the comparison, concat the rest of the ll or rl
    if l == lenOfllist:
        items.extend(rlist[r:lenOfrlist])
    else: # r == lenOf_rl
        items.extend(llist[l:lenOfllist])

    slist.put(items)# put the sorted list into the queue


    
    
    
def Process_bubble_merge(_list, slist):# doing bubblesort and mergesort in single process
    for l in _list:
        BubbleSort(l, slist)

    while slist.qsize() != 1:
        ll = slist.get()
        rl = slist.get()
        MergeSort(ll, rl, slist)

        
        
def WriteFile(fileName, slist, perf_time): # just write list into output file
    fileName = fileName[5:len(fileName)]
    file = open('output' + fileName, "w") # write the file
 
    file.write('Sorted:\n')
    for index in slist:
        file.write(str(index) + ' ')
    file.write('\nSort time : {:.5f} sec'.format(perf_time))
    print("Succeeded ! \"" + "output" + fileName + "\" has been created.")
    file.close()




# In[2]:

'''
Case :
1.Make N numbers with Bubble Sort and display the CPU execution time.

'''

def Case1(fileName,file):
    
    flist = GetList(fileName,file)
    slist = queue.Queue(len(flist))

    start = time.perf_counter() # clock start
    BubbleSort(flist,slist) # Start bubblesort function

    process_time = time.perf_counter() - start # clock end
    print('Time: ' + str(process_time))
    WriteFile(fileName, slist.get(), process_time)


# In[3]:

'''
Case :
2.Cut N numbers into k parts, and make the Bubble Sort by K threads, 
  Merge Sort by k-1 threads and display the CPU execution time.
  
'''

def Case2(fileName,f):
    threads = []
    m_threads = []
    sepNum = ''

    while not sepNum.isdigit():
         sepNum = input('Scanf the value k :')
    sepNum = int(sepNum)
    sep_list = CutList(fileName, sepNum, f) # get the processed seprated list
    slist = queue.Queue(sepNum) # a queue to store several small lists which are sorted by Bubblesort

    i,j = 0,0
    for i in range(sepNum):
        t = threading.Thread(name = 'bb_t' + str(i), target=BubbleSort, args=(sep_list[i], slist))
        threads.append(t)

    start = time.perf_counter() # clock start

    cnt,t = 0,0
    while t < sepNum or cnt < sepNum - 1: # it have to run k thread of bubble sort and k-1 thread of mergesort
        if t < sepNum: # start bubblesort
            threads[t].start()
            t += 1
        if slist.qsize() >= 2: # start mergesort only if the size of slist is larger than 1
            ll = slist.get()
            rl = slist.get()
            mt = threading.Thread(name = 'm_t' + str(cnt), target=MergeSort, args=(ll, rl, slist))
            mt.start()
            m_threads.append(mt)
            cnt += 1

    for m in m_threads: # wait for all thread are terminated
        m.join()
    
    process_time = time.perf_counter() - start # clock end

    print('Time: ' + str(process_time))
    WriteFile(fileName, slist.get(), process_time)


# In[4]:
'''
Case :
3.Cut N numbers into k parts, and make the Bubble Sort by K Processes,
  Merge Sort by k-1 Processes and display the CPU execution time.

'''

def Case3(fileName, f):
    processes = []
    m_processes = []
    sepNum = ''

    while not sepNum.isdigit():
         sepNum = input('Scanf the value k :')
    sepNum = int(sepNum)
    sep_list = CutList(fileName, sepNum, f) # get the processed seprated list
    manager = multiprocessing.Manager()
    slist = manager.Queue(sepNum) # a queue to store several lists which are sorted by Bubblesort

    for i in range(sepNum):
        p = multiprocessing.Process(name ='bb_p' + str(i + 1), target=BubbleSort, args=(sep_list[i], slist))
        processes.append(p)

    start = time.perf_counter() # clock start
    cnt, p = 0,0
    while p < sepNum or cnt < sepNum - 1: # it have to run k process of bubble sort and k-1 process of mergesort
        if p < sepNum: # start bubblesort
            processes[p].start()
            p += 1
        if slist.qsize() >= 2: # start mergesort only if the size of slist is larger than 1
            ll = slist.get()
            rl = slist.get()
            mp = multiprocessing.Process(name='m_p' + str(cnt + 1), target=MergeSort, args=(ll, rl, slist))
            mp.start()
            m_processes.append(mp)
            cnt += 1

    for mp in m_processes: # wait for all process are terminated
        mp.join()

    process_time = time.perf_counter() - start # clock end

    print('Time: ' + str(process_time))
    WriteFile(fileName, slist.get(), process_time)


# In[5]:
'''
Case :
4.Cut N numbers into k parts, and make the Bubble Sort on the K parts of data in a process,
  use the same process for Merge Sort and display the CPU execution time.

'''

def Case4(fileName, f):
    sepNum = ''
    while not sepNum.isdigit():
        sepNum = input('Scanf the value k :')
    sepNum = int(sepNum)
    sep_list = CutList(fileName, sepNum, f) # cut the list  
    manager = multiprocessing.Manager()
    slist = manager.Queue(sepNum) # a queue to store several lists which are sorted by Bubblesort

    start = time.perf_counter() # clock start

    # just put the separated list into the Process_bubble_merge function as a process
    pbm = multiprocessing.Process(target = Process_bubble_merge, args = (sep_list, slist))
    pbm.start()
    pbm.join()

    process_time = time.perf_counter() - start # clock end

    print('Sort time: ' + str(process_time) + 'sec' )
    WriteFile(fileName, slist.get(), process_time)


# In[ ]:


'''
資訊三甲 10627149 許峯僑
OS Project1

Case :
1.Make N numbers with Bubble Sort and display the CPU execution time.

2.Cut N numbers into k parts, and make the Bubble Sort by K threads, 
  Merge Sort by k-1 threads and display the CPU execution time.
  
3.Cut N numbers into k parts, and make the Bubble Sort by K Processes,
  Merge Sort by k-1 Processes and display the CPU execution time.
  
4.Cut N numbers into k parts, and make the Bubble Sort on the K parts of data in a process,
  use the same process for Merge Sort and display the CPU execution time.

'''

if __name__ == '__main__': 

 
    print("===========================")
    print("=       OS Project1       =")
    print("=1. Bubble Sort           =")
    print("=2. Thread Sort           =")
    print("=3. Process Sort          =")
    print("=4. One Process Sort      =")
    print("===========================")
	
    fileName = input('Scanf the File name: ')
    fileName = fileName + ".txt"
    file = open(fileName , 'r') # read the file
	#the first number is the case 
    case = file.read(1) # read the first number 
	
    if case == '1':
        Case1(fileName, file)
    elif case == '2':
        Case2(fileName, file)
    elif case == '3':
        Case3(fileName, file)
    elif case == '4':
        Case4(fileName, file)
    else :
        print("The case does not exist.\n") # error message with the case does not exist 
        print("Please enter the number in the range of 1~4")

    print("Sort complete !")
    
    


# In[ ]:




