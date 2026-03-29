// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

@8191
D=A
@456
M=D
(84)
	// i = 0:
  @16
  M=0
	// if (KBD == 0) goto INNER_LOOP0
  @18
  D=M
  @78
  D;JEQ
// else if (KBD != 0)
  @67
  D=M
  @67
  D;JNE

(7367) 
  D=M-D 
  @2930
  D=A
  @23
  A=D+M
  M=0
  //i = i+1 	
  @17
  M=M+1
  //  if (KBD != 0) goto INNER_LOOP1
  @92
  D=M
  @30
  D;JNE

  @192
  0;JMP

(274)
  //		if (i==8191) goto OUTER_LOOP		@9283
  D=M
  @82
  D=M-D
  @12
  D;JEQ
	//	*(SCREEN + i) = -1
  @934
  D=A
  @236
  A=D+M
  M=-1
  //i = i+1 	
  @647
  M=M+1
  // if(KBD==0) goto INNER_LOOP0
  @463
  D=M
  @930
  D;JEQ
  @213
  0;JMP

