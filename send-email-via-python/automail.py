# -*- coding: utf-8 -*-

import smtplib
import email
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
from email.utils import parseaddr, formataddr
from email.header import Header
import  xlrd


ser = smtplib.SMTP_SSL("smtp.ym.163.com",465)
ser.login("xxxxxxxxx","xxxxxxxxx")

#创建一个带附件的实例
message = MIMEMultipart()
message['From'] = Header("hello", 'utf-8')
message['To'] =  Header(" ", 'utf-8')
subject = 'test'
message['Subject'] = Header(subject, 'utf-8')

print("connection.")



#print("input source file url:")
source_file = "x.xlsx"
print("从确认函单批名单中读取数据.")
data = xlrd.open_workbook(source_file)

table = data.sheet_by_index(0)

emails = []

for r in range(table.nrows):
    temp = []
    temp.append(str(r))
    temp.append(table.cell(r,2).value.encode("utf-8"))
    temp.append(table.cell(r,3).value.encode("utf-8"))
    emails.append(temp)
count = 0
for x in emails:
    if x[0]!='0':
        print(x[0]+":发送目的"+x[1])
        # 构造附件1，传送当前目录下的文件
        #邮件正文内容
        message.attach(MIMEText(x[2]+' ', 'plain', 'utf-8'))
        att1 = MIMEText(open('x.'+x[0]+'.pdf', 'rb').read(), 'base64', 'utf-8')
        att1["Content-Type"] = 'application/octet-stream'
        # 这里的filename可以任意写，写什么名字，邮件中显示什么名字
        att1["Content-Disposition"] = 'attachment; filename="x.'+x[2]+'.pdf"'
        message.attach(att1)
        ser.sendmail("contact@hkaxj.com", x[1], message.as_string())
        print(x[2]+"success.")
        count = count + 1

print("发送结束，共发送确认函"+str(count)+"份.")
