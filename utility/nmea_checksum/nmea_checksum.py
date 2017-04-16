import sys
import re


def checksum( msg ):
    """
    Check the checksum of a NMEA 0183 sentence.
    Skips leading ! or $ and stops at '*'
    Ignores non-standard extensions after the checksum
    Ignores anything before the first $ or ! in the string
    """

    # Find the start of the NMEA sentence
    startchars = "!$"
    for c in startchars:
        i = msg.find(c)
        if i >= 0: break
    else:
        return (False, None, None)

    # Calculate the checksum on the message
    sum1 = 0
    for c in msg[i+1:]:
        if c == '*':
	    break;
        sum1 = sum1 ^ ord(c)
    sum1 = sum1 & 0xFF
    
    # Find the checksum at the end of the string
    sum2 = -1
    try:
        i = msg.index( '*' )
	sum2 = int( msg[i+1:i+3], 16)
	
	if sum1 == sum2:
	    return (True, sum1, sum2)
    except:
        pass

    return (False, sum1, sum2)


"""
Main
"""
try:
    inp_file_name   =   sys.argv[1]
    log_file_name   =   inp_file_name + ".err"
except:
    print "usage:       ", sys.argv[0], "input_file"
    sys.exit( 1 )

inp_file        =   open( inp_file_name, 'rb' )
log_file        =   open( log_file_name, 'w'  )  

print "converting...    ",

line_num        = 0
errors_count    = 0

for line in inp_file.readlines():
    line_num    += 1

    valide, given, expected = checksum( line )

    if expected != given:
        print "line: %8d, chksum is: %02X, calculated: %02X" % ( line_num, expected, given )
        log_file.write( line )
        errors_count += 1

inp_file.close()
log_file.close()

print "done"
print "lines, total:    ", line_num
print "errors, total:   ", errors_count
