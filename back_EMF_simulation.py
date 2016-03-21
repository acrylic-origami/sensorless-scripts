from scipy.integrate import dblquad, quad
import math as m
def integrand(r, z):
	R=0.0047825
	l=0.00635
	u0=4*m.pi/10**7
	J=0.4502*2*R**2*m.sqrt(l**2+R**2)/(2*m.pi*R**3*l*u0)
	k=(4*R*r)/((R+r)**2+z**2)
	return r*(u0*J*R)/(m.sqrt((R+r)**2+z**2))*(quad(lambda x: 1/m.sqrt(1-k*m.sin(x)**2), 0, m.pi/2)[0]+(R**2-r**2-z**2)/((R-r)**2+z**2)*quad(lambda x: m.sqrt(1-k*m.sin(x)**2), 0, m.pi/2)[0])



for j in range(-73 , 100):
	if j!=0:
		sn=0
		ss=0
		a=0.000101*j
		errn=0
		errs=0
		d=0.01905 # separation b
		for k in range(0, 5):
			for i in range(0, 55):
				for l in range(0, 5):
					int1=quad(lambda x: integrand(x, a+i*(0.00562/55)+l*2*d), 0, 0.007785+k*0.000101)
					int2=quad(lambda x: integrand(x, a+i*(0.00562/55)+l*2*d+0.00635), 0, 0.007785+k*0.000101)
					sn+=2*m.pi*(int1[0]-int2[0])
					errn+=2*m.pi*(int1[1]+int2[1])
				for l in range(0, 5):
					int1=quad(lambda x: integrand(x, a+d+i*(0.00562/55)+l*2*d), 0, 0.007785+k*0.000101)
					int2=quad(lambda x: integrand(x, a+d+i*(0.00562/55)+l*2*d+0.00635), 0, 0.007785+k*0.000101)
					ss+=2*m.pi*(int1[0]-int2[0])
					errs+=2*m.pi*(int1[1]+int2[1])

		for i in range(0, 5):
			for l in range(0, 5):
				int1=quad(lambda x: integrand(x, a+i*(0.00562/5)+l*2*d), 0, 0.007785+k*0.000101)
				int2=quad(lambda x: integrand(x, a+i*(0.00562/5)+l*2*d+0.00635), 0, 0.007785+k*0.000101)
				sn+=2*m.pi*(int1[0]-int2[0])
				errn+=2*m.pi*(int1[1]+int2[1])
			for l in range(0, 5):
				int1=quad(lambda x: integrand(x, a+d+i*(0.00562/5)+l*2*d), 0, 0.007785+k*0.000101)
				int2=quad(lambda x: integrand(x, a+d+i*(0.00562/5)+l*2*d+0.00635), 0, 0.007785+k*0.000101)
				ss+=2*m.pi*(int1[0]-int2[0])
				errs+=2*m.pi*(int1[1]+int2[1])
		s=sn-ss
		err=errn+errs
		#for i in range(0, 5):
		'''int1=quad(lambda x: integrand(x, a+i*(0.00562/5)), 0, 0.008290)
			int2=quad(lambda x: integrand(x, a+i*(0.00562/5)+0.00635), 0, 0.008290)
			s+=2*m.pi*(int1[0]-int2[0])
			err+=2*m.pi*int1[1]+int2[1]'''
		#print str(a)+'\t'+str(2*m.pi*(quad(lambda x: integrand(x, a), 0, 0.004)[0]-quad(lambda x: integrand(x, a+0.00635), 0, 0.004)[0]))
		print str(a)+'\t'+str(s)+'\t'+str(err)