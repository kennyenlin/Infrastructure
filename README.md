Infrastructure
=============

Overview
-------
This project provides a method to transform data of coffee_shops in Australia into data base <br>
and it also provide methods to locate nearest coffee shop by its location and/or given partial name<br>

Environment
--------
Proramming Language: C and C++ <br>
IDE:                 Visual Studio 2008 <br>

Approach
--------
1. import files and separate it into tokens with different criteria <br>
2. after retrieving the data, use given architecture to store them into file-based database SQLite <br>
3. request the data through its location and/or partial-name and sorted by distance <br>

Assumption
--------
By given sample files, there are some assumptions in the schemes of csv retrievement. <br>
1. First two elements are all type double and these two fields are necessary <br>
2. In the following, all the fields should be string and enclosed by "" <br>

User Guide
--------
1. open the solution file Infrastructure.sln with visual studio 2008 <br>
2. compile it with "build" option <br>
3. entering "Release" folder, there is a Infrastructure.exe and two sample files, called coffee_shops-1.csv and coffee_shops-2.csv <br>
4. open command prompt window and type request <br>
5. request form is like "Infrastructure.exe LAT=30.7 LNG=150.2 NAME=coffee" <br>
6. you could replace the number and partial name but the format should be like above. <br>
7. LAT, LNG, NAME are fixed. Any changes would not be allowed to query. Also, there is no space or other notations beside "=" <br>
8. result would show in order of distance if location is provided. If not, data would show in the order of database. Also, If partial name is provided, result would be filtered <br>
