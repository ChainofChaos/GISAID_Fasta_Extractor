# GISAID Fasta Extractor
A tool to filter the full fasta sequences from GISAID. The tool will separate the fasta sequences based on the country and provide the statistic in CSV format based on the date provided in the sequence.

This will only works for the full sequences provided by GISAID.

# Usage
```
GISAID_Fasta_Extractor [options] fastafile
```
## Options
`-h --help` shows help message and exits\
`-v --version`   prints version information and exits\
`fastafile`            specify the fastafile path

## Example
```
./GISAID_Fasta_Extractor fullprot.fasta
```
