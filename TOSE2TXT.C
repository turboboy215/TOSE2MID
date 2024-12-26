/*TOSE (GB/GBC) to MIDI converter*/
/*By Will Trowbridge*/
/*Portions based on code by ValleyBell*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define bankSize 16384

FILE* rom, * txt, * cfg;
long bank;
long tablePtrLoc;
long tableOffset;
long tempoTablePtrLoc;
long tempoTableOffset;
int i, j;
char outfile[1000000];
int songNum;
int numSongs;
long songPtr = 0;
int numChan = 0;
long songPtrs[4];
long curSpeed;
long bankAmt;
long nextPtr;
int masterBank;
int cfgPtr = 0;
int exitError = 0;
int fileExit = 0;
int foundTable = 0;
long base = 0;
int curTrack;
int format = 1;
int speedVal = 5;
int hasSub = 0;
int sub = 1;
long firstSub = 0;

char string1[100];
char string2[100];
char checkStrings[7][100] = { "numSongs=", "format=", "bank=", "addr=", "chan=", "speedval=", "sub="};
unsigned static char* romData;
unsigned static char* exRomData;
unsigned static char* cfgData;

/*Function prototypes*/
unsigned short ReadLE16(unsigned char* Data);
unsigned short ReadBE16(unsigned char* Data);
static void Write8B(unsigned char* buffer, unsigned int value);
static void WriteBE32(unsigned char* buffer, unsigned long value);
static void WriteBE24(unsigned char* buffer, unsigned long value);
static void WriteBE16(unsigned char* buffer, unsigned int value);
void song2txt(int songNum, long ptr, int chanNums);

/*Convert little-endian pointer to big-endian*/
unsigned short ReadLE16(unsigned char* Data)
{
	return (Data[0] << 0) | (Data[1] << 8);
}

/*Store big-endian pointer*/
unsigned short ReadBE16(unsigned char* Data)
{
	return (Data[0] << 8) | (Data[1] << 0);
}

static void Write8B(unsigned char* buffer, unsigned int value)
{
	buffer[0x00] = value;
}

static void WriteBE32(unsigned char* buffer, unsigned long value)
{
	buffer[0x00] = (value & 0xFF000000) >> 24;
	buffer[0x01] = (value & 0x00FF0000) >> 16;
	buffer[0x02] = (value & 0x0000FF00) >> 8;
	buffer[0x03] = (value & 0x000000FF) >> 0;

	return;
}

static void WriteBE24(unsigned char* buffer, unsigned long value)
{
	buffer[0x00] = (value & 0xFF0000) >> 16;
	buffer[0x01] = (value & 0x00FF00) >> 8;
	buffer[0x02] = (value & 0x0000FF) >> 0;

	return;
}

static void WriteBE16(unsigned char* buffer, unsigned int value)
{
	buffer[0x00] = (value & 0xFF00) >> 8;
	buffer[0x01] = (value & 0x00FF) >> 0;

	return;
}

int main(int args, char* argv[])
{
	printf("TOSE (GB/GBC) to TXT converter\n");
	if (args != 3)
	{
		printf("Usage: TOSE2TXT <rom> <config>\n");
		return -1;
	}
	else
	{
		if ((rom = fopen(argv[1], "rb")) == NULL)
		{
			printf("ERROR: Unable to open ROM file %s!\n", argv[1]);
			exit(1);
		}
		else
		{
			if ((cfg = fopen(argv[2], "r")) == NULL)
			{
				printf("ERROR: Unable to open configuration file %s!\n", argv[1]);
				exit(1);
			}
			else
			{
				/*Get the total number of songs*/
				fgets(string1, 10, cfg);
				if (memcmp(string1, checkStrings[0], 1))
				{
					printf("ERROR: Invalid CFG data!\n");
					exit(1);

				}
				fgets(string1, 4, cfg);
				numSongs = strtod(string1, NULL);
				printf("Total # of songs: %i\n", numSongs);

				/*Skip new line*/
				fgets(string1, 2, cfg);
				/*Get the format number*/
				fgets(string1, 8, cfg);

				if (memcmp(string1, checkStrings[1], 1))
				{
					printf("ERROR: Invalid CFG data!\n");
					exit(1);

				}
				fgets(string1, 3, cfg);
				format = strtod(string1, NULL);
				printf("Format: %i\n", format);

				/*Sanrio Timenet has dynamic sub-songs*/
				if (format == 8)
				{
					hasSub = 1;
					format = 3;
				}

				/*Skip new line*/
				fgets(string1, 2, cfg);
				/*Get the base speed value*/
				fgets(string1, 10, cfg);

				if (memcmp(string1, checkStrings[5], 1))
				{
					printf("ERROR: Invalid CFG data!\n");
					exit(1);

				}
				fgets(string1, 3, cfg);
				speedVal = strtod(string1, NULL);
				printf("Base speed value: %i\n", speedVal);

				for (songNum = 1; songNum <= numSongs; songNum++)
				{
					/*Skip new line*/
					fgets(string1, 2, cfg);

					/*Skip song number*/
					fgets(string1, 11, cfg);

					/*Get the bank number*/
					fgets(string1, 6, cfg);
					if (memcmp(string1, checkStrings[2], 1))
					{
						printf("ERROR: Invalid CFG data!\n");
						exit(1);

					}
					fgets(string1, 4, cfg);
					bank = strtol(string1, NULL, 16);

					/*Copy the current bank's ROM data*/
					if (bank != 1)
					{
						bankAmt = bankSize;
						fseek(rom, ((bank - 1) * bankSize), SEEK_SET);
						romData = (unsigned char*)malloc(bankSize);
						fread(romData, 1, bankSize, rom);
					}

					else
					{
						if (format == 7)
						{
							/*Banks 1 and 6 are used together for Play Action Football*/
							bankAmt = 0;
							fseek(rom, 0, SEEK_SET);
							romData = (unsigned char*)malloc(bankSize * 2);
							fread(romData, 1, bankSize, rom);
							fseek(rom, (5 * bankSize), SEEK_SET);
							fread(romData + bankSize, 1, bankSize, rom);
						}

						else
						{
							bankAmt = 0;
							fseek(rom, ((bank - 1) * bankSize * 2), SEEK_SET);
							romData = (unsigned char*)malloc(bankSize * 2);
							fread(romData, 1, bankSize * 2, rom);
						}
					}

					/*Skip new line*/
					fgets(string1, 2, cfg);

					/*Get the hex address*/
					fgets(string1, 6, cfg);
					if (memcmp(string1, checkStrings[3], 1))
					{
						printf("ERROR: Invalid CFG data!\n");
						exit(1);

					}
					fgets(string1, 5, cfg);
					songPtr = strtol(string1, NULL, 16);

					/*Skip new line*/
					fgets(string1, 2, cfg);

					/*Get the number of channels*/
					fgets(string1, 6, cfg);
					if (memcmp(string1, checkStrings[4], 1))
					{
						printf("ERROR: Invalid CFG data!\n");
						exit(1);

					}
					fgets(string1, 3, cfg);
					numChan = strtol(string1, NULL, 16);

					/*Special case for Sanrio Timenet sub-songs*/
					if (hasSub == 1)
					{
						/*Skip new line*/
						fgets(string1, 2, cfg);

						/*Get the number of channels*/
						fgets(string1, 5, cfg);

						if (memcmp(string1, checkStrings[6], 1))
						{
							printf("ERROR: Invalid CFG data!\n");
							exit(1);

						}
						fgets(string1, 3, cfg);
						sub = strtol(string1, NULL, 16);
					}

					printf("Song %i: bank %01X, address 0x%04X, channels: %i\n", songNum, bank, songPtr, numChan);
					song2txt(songNum, songPtr, numChan);

				}

				printf("The operation was successfully completed!\n");
				return 0;
			}
		}

	}
}

void song2txt(int songNum, long ptr, int chanNums)
{
	int curTrack = 0;
	int ptrs[4] = { 0, 0, 0, 0 };
	long command[5];
	int chanMask = 0;
	int chanNum = 0;
	long romPos = 0;
	long seqPos = 0;
	int seqEnd = 0;
	int chanSpeed = 0;
	int chanVol = 0;
	int parameters[4] = { 0, 0, 0, 0 };
	unsigned char lowNibble = 0;
	unsigned char highNibble = 0;
	int curNote = 0;
	int curNoteLen = 0;
	int octave = 0;
	int repeatTimes = 0;
	int repeatAmt = 0;
	long macroPos = 0;
	long macroRet = 0;
	int macroTimes = 0;
	
	sprintf(outfile, "song%d.txt", songNum);
	if ((txt = fopen(outfile, "wb")) == NULL)
	{
		printf("ERROR: Unable to write to file song%d.txt!\n", songNum);
		exit(2);
	}
	else
	{
		romPos = ptr - bankAmt;
		for (curTrack = 0; curTrack < chanNums; curTrack++)
		{
			chanMask = romData[romPos];
			chanNum = romData[romPos + 1];
			ptrs[chanNum] = ReadLE16(&romData[romPos + 2]);
			fprintf(txt, "Channel %i: 0x%04X\n", (chanNum + 1), ptrs[chanNum]);
			fprintf(txt, "Channel mask: %01X\n", chanMask);
			fprintf(txt, "\n");
			romPos += 4;
		}

		fprintf(txt, "\n");

		for (curTrack = 0; curTrack < 4; curTrack++)
		{
			if (ptrs[curTrack] != 0)
			{
				seqPos = ptrs[curTrack] - bankAmt;
				parameters[0] = romData[seqPos];
				parameters[1] = romData[seqPos + 1];
				parameters[2] = romData[seqPos + 2];
				parameters[3] = romData[seqPos + 3];
				fprintf(txt, "Channel %i:\n", (curTrack + 1));
				chanSpeed = parameters[0];
				fprintf(txt, "Parameters: speed = %01X, duty = %01X, volume = %01X, sweep/wave = %01X\n", parameters[0], parameters[1], parameters[2], parameters[3]);
				seqPos += 4;

				seqEnd = 0;
				while (seqEnd == 0)
				{
					command[0] = romData[seqPos];
					command[1] = romData[seqPos + 1];
					command[2] = romData[seqPos + 2];
					command[3] = romData[seqPos + 3];
					command[4] = romData[seqPos + 4];

					if (command[0] < 0xA0)
					{
						lowNibble = (command[0] >> 4);
						highNibble = (command[0] & 15);
						octave = lowNibble;
						curNote = highNibble;
						curNoteLen = command[1];
						fprintf(txt, "Play note: %01X, octave: %01X, length: %01X\n", curNote, octave, curNoteLen);
						seqPos += 2;

					}

					else if (command[0] == 0xA0)
					{
						fprintf(txt, "Set envelope length: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA1)
					{
						fprintf(txt, "Set panning (v1)?: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA2)
					{
						fprintf(txt, "Set duty: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA3)
					{
						fprintf(txt, "Set panning (v2)?: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA4)
					{
						fprintf(txt, "Set panning (v3)?: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA5)
					{
						fprintf(txt, "Set envelope?: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA6)
					{
						fprintf(txt, "Set global volume: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA7)
					{
						fprintf(txt, "Set delay: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xA8)
					{
						if (format != 4)
						{
							fprintf(txt, "Command A8: %01X\n", command[1]);
							seqPos += 2;
						}
						else if (format == 4)
						{
							macroTimes = romData[seqPos + 1];
							macroPos = ReadLE16(&romData[seqPos + 2]);
							macroRet = seqPos + 4;
							fprintf(txt, "Go to macro 0x%04X, %i times\n", macroPos, macroTimes);
							seqPos += 4;
						}

					}

					else if (command[0] == 0xA9)
					{
						if (format != 4)
						{
							macroPos = ReadLE16(&romData[seqPos + 2]);
							fprintf(txt, "Macro pattern, start: %04X\n", macroPos);
							seqPos += 2;
						}
						else if (format == 4)
						{
							fprintf(txt, "Return from macro\n");
							seqPos += 2;
						}

					}

					else if (command[0] == 0xAC)
					{
						macroTimes = command[1];
						macroPos = ReadLE16(&romData[seqPos + 2]);
						fprintf(txt, "Jump to position: %04X, %01X times\n", macroPos, macroTimes);
						seqPos += 4;
					}

					else if (command[0] == 0xAD)
					{
						fprintf(txt, "Return from jump\n");
						seqPos += 2;
					}

					else if (command[0] == 0xAE)
					{
						fprintf(txt, "Set tuning: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xAF)
					{
						chanSpeed = command[1];
						fprintf(txt, "Set channel speed: %01X\n", chanSpeed);
						seqPos += 2;
					}

					else if (command[0] >= 0xB0 && command[0] < 0xC0)
					{
						highNibble = (command[0] & 15);
						repeatTimes = highNibble;
						repeatAmt = command[1];

						if (format == 0 || format == 1 || format == 2)
						{
							if (repeatTimes > 0)
							{
								fprintf(txt, "Repeat: %i times, amount: %01X\n", repeatTimes, repeatAmt);
								seqPos += 2;
							}
							else if (repeatTimes == 0)
							{
								fprintf(txt, "Repeat: infinite times, amount: %01X\n\n", repeatAmt);
								seqEnd = 1;
							}
						}
						else
						{
							if (command[1] == 0xF1)
							{
								if (repeatTimes > 0)
								{
									fprintf(txt, "Repeat: %i times, position: 1\n", repeatTimes);
									seqPos += 2;
								}
								else if (repeatTimes == 0)
								{
									fprintf(txt, "Repeat: infinite times, position: 1\n\n");
									seqEnd = 1;
								}
							}
							else if (command[1] == 0xF2)
							{
								if (repeatTimes > 0)
								{
									fprintf(txt, "Repeat: %i times, position: 2\n", repeatTimes);
									seqPos += 2;
								}
								else if (repeatTimes == 0)
								{
									fprintf(txt, "Repeat: infinite times, position: 2\n\n");
									seqEnd = 1;
								}
							}
							else if (command[1] == 0xF3)
							{
								if (repeatTimes > 0)
								{
									fprintf(txt, "Repeat: %i times, position: 3\n", repeatTimes);
									seqPos += 2;
								}
								else if (repeatTimes == 0)
								{
									fprintf(txt, "Repeat: infinite times, position: 3\n\n");
									seqEnd = 1;
								}
							}
							else if (command[1] == 0xF0)
							{
								if (repeatTimes > 0)
								{
									fprintf(txt, "Repeat: %i times, position: 0\n", repeatTimes);
									seqPos += 2;
								}
								else if (repeatTimes == 0)
								{
									fprintf(txt, "Repeat: infinite times, position: 0\n\n");
									seqEnd = 1;
								}
							}
							else if (command[1] == 0xFC)
							{
								repeatAmt = ReadLE16(&romData[seqPos + 2]);
								if (repeatTimes > 0)
								{
									fprintf(txt, "Repeat: %i times, go to position: %04X\n", repeatTimes, repeatAmt);
									seqPos += 2;
								}
								else if (repeatTimes == 0)
								{
									fprintf(txt, "Repeat: infinite times, go to position: %04X\n\n", repeatAmt);
									seqEnd = 1;
								}
							}
							else
							{
								repeatAmt = command[1] - 0xF0;
								if (repeatTimes > 0)
								{
									fprintf(txt, "Repeat: %i times, position: %i\n", repeatTimes, repeatAmt);
									seqPos += 2;
								}
								else if (repeatTimes == 0)
								{
									fprintf(txt, "Repeat: infinite times, position: %i\n\n", repeatAmt);
									seqEnd = 1;
								}
							}

						}

						
					}

					else if (command[0] >= 0xC0 && command[0] < 0xFD)
					{
						fprintf(txt, "Noise/effect settings: %01X, %01X\n", command[0], command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xFD)
					{
						fprintf(txt, "Start of repeat: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xFE)
					{
						fprintf(txt, "Skip?: %01X\n", command[1]);
						seqPos += 2;
					}

					else if (command[0] == 0xFF)
					{
						fprintf(txt, "End of track\n\n");
						seqEnd = 1;
					}

					else
					{
						fprintf(txt, "Unknown command: %01X, %01X\n", command[0], command[1]);
						seqPos += 2;
					}
				}
			}
		}
	}
}