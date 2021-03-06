#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
""":synopsis: Lock and queue for mutual exclusion.
:deprecated:

"""
class mutex:


	"""
	Create a new (unlocked) mutex.
	
	A mutex has two pieces of state --- a "locked" bit and a queue. When the mutex
	is not locked, the queue is empty. Otherwise, the queue contains zero or more
	``(function, argument)`` pairs representing functions (or methods) waiting to
	acquire the lock. When the mutex is unlocked while the queue is not empty, the
	first queue entry is removed and its  ``function(argument)`` pair called,
	implying it now has the lock.
	
	Of course, no multi-threading is implied -- hence the funny interface for
	:meth:`lock`, where a function is called once the lock is acquired.
	
	
	.. utex Objects
	-------------
	
	:class:`mutex` objects have following methods:
	
	
	"""
	
	
	def __init__(self, ):
		pass
	
	def test(self, ):
		"""
		Check whether the mutex is locked.
		
		
		"""
		pass
		
	def testandset(self, ):
		"""
		"Atomic" test-and-set, grab the lock if it is not set, and return ``True``,
		otherwise, return ``False``.
		
		
		"""
		pass
		
	def lock(self, function,argument):
		"""
		Execute ``function(argument)``, unless the mutex is locked. In the case it is
		locked, place the function and argument on the queue. See :meth:`unlock` for
		explanation of when ``function(argument)`` is executed in that case.
		
		
		"""
		pass
		
	


