#!/usr/bin/env python2.7
# -*- coding: utf-8 -*-
""":synopsis: Read and write strings as if they were files.


This module implements a file-like class, :class:`StringIO`, that reads and
writes a string buffer (also known as *memory files*).  See the description of
file objects for operations (section :ref:`bltin-file-objects`). (For
standard strings, see :class:`str` and :class:`unicode`.)


"""
class StringIO:


	"""
	When a :class:`StringIO` object is created, it can be initialized to an existing
	string by passing the string to the constructor. If no string is given, the
	:class:`StringIO` will start empty. In both cases, the initial file position
	starts at zero.
	
	The :class:`StringIO` object can accept either Unicode or 8-bit strings, but
	mixing the two may take some care.  If both are used, 8-bit strings that cannot
	be interpreted as 7-bit ASCII (that use the 8th bit) will cause a
	:exc:`UnicodeError` to be raised when :meth:`getvalue` is called.
	
	The following methods of :class:`StringIO` objects require special mention:
	
	
	"""
	
	
	def __init__(self, ):
		pass
	
	def getvalue(self, ):
		"""
		Retrieve the entire contents of the "file" at any time before the
		:class:`StringIO` object's :meth:`close` method is called.  See the note above
		for information about mixing Unicode and 8-bit strings; such mixing can cause
		this method to raise :exc:`UnicodeError`.
		
		
		"""
		pass
		
	def close(self, ):
		"""
		Free the memory buffer.  Attempting to do further operations with a closed
		:class:`StringIO` object will raise a :exc:`ValueError`.
		
		Example usage::
		
		import StringIO
		
		output = StringIO.StringIO()
		output.write('First line.\n')
		print >>output, 'Second line.'
		
		# Retrieve file contents -- this will be
		# 'First line.\nSecond line.\n'
		contents = output.getvalue()
		
		# Close object and discard memory buffer --
		# .getvalue() will now raise an exception.
		output.close()
		
		
		:mod:`cStringIO` --- Faster version of :mod:`StringIO`
		======================================================
		
		"""
		pass
		
	


