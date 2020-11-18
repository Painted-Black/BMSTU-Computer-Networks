import smtplib
import argparse
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
import os
import magic
import mimetypes

MESSAGE = "Hello, see attached files!"
SUBJECT = "Hello files"

# python3 main.py --r=syzygy.sys@gmail.com --s=novocaine2121@gmail.com --p=Bjauno1998 --dir=./search_dir --keyword=Hello

def parse_args():
	argparser = argparse.ArgumentParser(description='Python SMTP client using smtplib module')
	argparser.add_argument("--receiver", help="Receiver address")
	argparser.add_argument("--sender", help="Sender address")
	argparser.add_argument("--pswd", help="Sender password")
	argparser.add_argument("p", help="Test")
	argparser.add_argument("--keyword", help="Keyword to search in txt files")
	argparser.add_argument("--dir", help="Path to dir where to find txt files with keyword")
	args = argparser.parse_args()
	return args

def check_args(args):
	args_ok = False
	try:
		if (args.receiver != None and args.sender != None and 
			args.pswd != None and args.keyword != None and 
			args.dir != None):
			args_ok = True
	except AttributeError:
		pass
	return args_ok

def check_file(filename, kwd):
	is_ok = False
	with open(filename, "r") as f:
		text = f.read()
		if kwd in text:
			is_ok = True
	return is_ok

def attach_file(msg, full_filename, filename):
	print("Attaching file " + full_filename)
	with open(full_filename, "r") as fp:
		file = MIMEText(fp.read())
		fp.close()

	file.add_header('Content-Disposition', 'attachment', filename=filename)
	msg.attach(file) 
	return msg

def get_file_type(filename):
	_type, _subtype = mimetypes.guess_type(filename)
	if (_type != None):
		return _type
	ftype = None
	try:
		ftype = magic.from_file(filename, mime=True)
	except IsADirectoryError:
		ftype = "dir"
	return ftype

def attach_files(msg, _dir, kwd):
	files = os.listdir(_dir)
	print("Found " + str(len(files)) + 
		  " files in folder " + _dir)
	
	for filename in files:
		full_filename = _dir + os.sep + filename
		ftype = get_file_type(full_filename)
		if ftype == "text/plain":
			if check_file(full_filename, kwd) == True:
				msg = attach_file(msg, full_filename, filename)	

	return msg

def send_email(msg, host, pswd):
	server = smtplib.SMTP(host) # create server
	
	server.starttls() # Puts the connection to the SMTP server into TLS mode
	# Transport Layer Security (TLS) is cryptographic protocol designed to provide communications security over a computer network	

	# Login Credentials for sending the mail
	server.login(msg['From'], pswd)
	server.sendmail(msg['From'], msg['To'], msg.as_string())
	server.quit()
	#try:
	#except smtplib.SMTPAuthenticationError:
		# print("Authentication failed!")
		#return -1
	
	# send the message via the server.

def main():
	args = parse_args()
	args_ok = check_args(args)
	if not args_ok:
		print("Incorrect command line arguments!")
		return

	# create message object instance
	msg = MIMEMultipart()
	message = MESSAGE

	# setup the parameters of the message
	password = args.pswd
	msg['From'] = args.sender
	msg['To'] = args.receiver
	msg['Subject'] = SUBJECT

	# add in the message body
	msg.attach(MIMEText(message, 'plain'))
	msg = attach_files(msg, args.dir, args.keyword)

	is_ok = None
	try:
		is_ok = send_email(msg, 'smtp.gmail.com: 587', args.pswd)
		print("Successfully send message to " + msg['To'])
	except Exception as e:
		print('Something went wrong, cannot send message!')
		print(e)
	if (is_ok != None):
		print("The following recepients were rejected: ")
		print(is_ok)
	# Порт для TLS/STARTTLS: 587
	# see https://support.google.com/mail/answer/7126229?hl=ru

if __name__ == "__main__":
	main()
