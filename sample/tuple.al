int a = 1, b = 2, c = 3;
string s = "a : %d, b : %d, c : %d\n";
printf(s.c_str(),a,b,c);
(a,b,c) = (b,c,a);
printf(s.c_str(),a,b,c);