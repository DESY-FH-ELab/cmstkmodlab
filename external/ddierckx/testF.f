      subroutine fortfunc1(ii,ff)
      integer ii
      real*4  ff

      write(6,100) ii, ff
 100  format('ii=',i2,' ff=',f6.3)

      return
      end

      subroutine fortfunc2(ii,ff)
      integer ii
      real*4  ff(ii)

      write(6,200) ii, ff(5)
 200  format('ii=',i2,' ff=',f6.3)

      return
      end
      