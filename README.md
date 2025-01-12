# simba_spectra_test
Parsing pcap format to save udp broadcasted data simba spectra messages to JSON


## Test Task:
1. Develop a parser for the pcap format, which is used to store network traffic, without using any third-party libraries.
1. Create a binary protocol decoder based on the Simba format specification Moscow exchange moex). The protocol specification can be found at http://ftp.moex.ru/pub/SIMBA/Spectra/prod/doc/spectra_simba_en.pdf. Focus on decoding messages like OrderUpdate, OrderExecution, and OrderBookSnapshot.
1. Test your decoder against the provided data at http://ftp.moex.ru/pub/SIMBA/Spectra/prod/pcap/ using the results obtained in part 1. Save the decoded output to a file in JSON format, with each input packet represented by a single line.

**Note:** Please feel free to ask any questions, while keeping in mind that the purpose of this test task is to showcase your skills and expertise.

## Observations:

1. pcap format has been described separately from spectra simba format
1. pcap files is being provided for testing and hence it was necessary to dig both formats
1. It was kinda interesting to dig in, however the clarification about test data format would be usefull even before start of work
1. 'root block' it is the question! How to get message ID from 'root block'?

## Assumptions

1. pcap format parsing will be skipped unless minimal data needs
1. we need produce fastest code to generate json data
1. result json data will be segregated to streams separated by data from EthernetII level and UDP port
1. result json data will not be validated complitely (UDP checksum calculation will be skipped, for example)
1. result json data will contain only OrderUpdate, OrderExecution, and OrderBookSnapshot records information. And, possible, names of other messages
1. initial version will collect only separated UDP records - Splitted messages concatenation will be skipped

## LOG
[01.01.2025] Time spent 3 working days
[05.01.2025] Time spent 4 working days
[12.01.2025] Time spent 6 working days 
 - threads for parallel processing has been added
 - Incremental packets processing done
 - SnapShot formatted packets - started, but need to be clarified





