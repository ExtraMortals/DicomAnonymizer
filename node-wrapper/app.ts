import {DcmAnon, patientInfo} from './dicomanon'

const dcm = new DcmAnon(__dirname + '/../sample.dcm');

dcm.makeAnonFile(patientInfo.PatientAge | patientInfo.PatientName).save("ftp://192.168.1.70/eflkefk.txt");