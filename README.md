DemoCrypt
=============================
A tiny crypting program written in C
------------------------------
About Software
==============================
DemoCrypt is a tiny crypting console program, which is also fast as much
as possible, The algorithm used for crypting is symmetric-key algorithm
based on bitwise XOR so the encrypting-key is also decrypting-key.

DemoCrypt stores the XOR of passwords MD5 and password in first 16 bytes
of ecrypted file for checking the password before decrypting and also uses
the next 16 bytes for header properties (currently empty).

This software is published with known isuses for cracking password by
bruteforce, finding password using known part of main file. some of them 
will be fixed in next versions.

Using a KeyFile instead of password can bring an acceptable security for
the encrypted file. 

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
