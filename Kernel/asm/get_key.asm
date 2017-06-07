GLOBAL read_key

read_key:
	in al,60h ; accede al teclado
	ret