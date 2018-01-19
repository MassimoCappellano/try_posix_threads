
all: pthread1 mutex1 join1 cond1 cond_timedwait

pthread1: pthread1.c
	cc pthread1.c -lpthread -o pthread1

mutex1: mutex1.c
	cc mutex1.c -lpthread -o mutex1

join1: join1.c
	cc join1.c -lpthread -o join1

cond1: cond1.c
	cc cond1.c -lpthread -o cond1

cond_timedwait: cond_timedwait.c
	cc cond_timedwait.c -lpthread -o cond_timedwait

clean:
	rm pthread1 mutex1 join1 cond1 cond_timedwait




