  # A combination or sed(1), gawk(1) and a C program for parsing Ethernet MAC addresses from a file and lookup the vendor of the chip

  oui.sh is the driving script, it accepts 2 arguments, the 
  2nd is optional, the first is the file where the mac 
  addresses are, and the second if it is "-d" it will use
  the httpoui.c http client to download the oui.txt from
  the IEEE. The oui.txt contain a list of OUI (3 octets) that
  list the vendor assignated to that OUI by the IEEE.
 
  IF the file oui.txt doesn't exist and no "-d" parameter was
  passed then the script will use a gawk(1) socket to make
  an http request to download the file, if "-d" was passed 
  the script will use make(1) to build the http client and
  use it to download the oui.txt file. After that sed(1) takes
  care to do some cleanups and then hand the data over to
  gawk(1) via a pipe(2) to continue filtering, finally after
  all is done the OUIs on the input mac file will be matched
  against the OUIs in the oui.txt file to find the vendor.

  NOTE: this small hobby project is developed on FreeBSD 11.2
        for that reason the http client in C might not build
        or work on any other unix-like OS since it make uses
        of at least one FreeBSD specific flag for getaddrinfo(2)
        I plan to fix this soon, it works, however in OpenBSD
        and NetBSD but not on Linux.

  Orestes Leal Rodriguez, 2018 <olealrd1981@gmail.com>
