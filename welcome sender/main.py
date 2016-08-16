import  doc
import readform
import automail


if __name__ == '__main__':
    data = readform.formparser().date2dic()
    for p in data:
        doc.make_new(p)
        import time
        #time.sleep(1000000.0)
        print "wake up"
        automail.SendMail(p)


