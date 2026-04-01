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
@total
M=D
(Outer_Loop)
	// i = 0:
  @i
  M=0
	// if (KBD == 0) goto INNER_LOOP0
  @KBD
  D=M
  @INNER_LOOP0
  D;JEQ
// else if (KBD != 0)
  @KBD
  D=M
  @INNER_LOOP1
  D;JNE

(INNER_LOOP0)
	// if(i==8191) go to OUTER_LOOP
  @i
  D=M
  @total
  D=M-D
  @Outer_Loop
  D;JEQ
	//	*(SCREEN + i) = 0
  @SCREEN
  D=A
  @i
  A=D+M
  M=0
  //i = i+1 	
  @i
  M=M+1
  //  if (KBD != 0) goto INNER_LOOP1
  @KBD
  D=M
  @INNER_LOOP1
  D;JNE

  @INNER_LOOP0
  0;JMP

(INNER_LOOP1)
  //		if (i==8191) goto OUTER_LOOP		
  @i
  D=M
  @total
  D=M-D
  @Outer_Loop
  D;JEQ
	//	*(SCREEN + i) = -1
  @SCREEN
  D=A
  @i
  A=D+M
  M=-1
  //i = i+1 	
  @i
  M=M+1
  // if(KBD==0) goto INNER_LOOP0
  @KBD
  D=M
  @INNER_LOOP0
  D;JEQ
  @INNER_LOOP1
  0;JMP

