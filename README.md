DemoCrypt
=============================
A tiny crypting program written in C
------------------------------
About Software
==============================
DemoCrypt is a tiny crypting console program, which is also fast as much
as possible, The algorithm used for crypting is symmetric-key algorithm
based on bitwise XOR so the encrypting-key is also decrypting-key.

DemoCrypt stores the MD5 of password in first 32 Bytes of ecrypted file,
for checking the password before decrypting. 

This software is published with known isuses for cracking password like
cracking MD5, finding password using known part of main file. some of them
will be fixed in next versions.

License
==============================
Copyright (C) 2015 Hesam Haddad

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program.  If not, see http://www.gnu.org/licenses/.
