import * as ffi from 'ffi-napi'
import * as iconv from 'iconv-lite'
import * as fs from 'fs'

const libpath = process.platform == 'linux' ?
    `${__dirname}/../lib/${process.platform}//libdcmanon` :
    `${__dirname}\\..\\lib\\${process.platform}\\dcmanon.dll`

export enum patientInfo {
	PatientID           = 1 << 0,
	PatientName         = 1 << 1,
	PatientBirthDate    = 1 << 2,

	PatientBirthTime    = 1 << 3,
	PatientAge          = 1 << 4,
	PatientSex          = 1 << 5,
	PatientWeight       = 1 << 6,
	PatientSize         = 1 << 7,
	PatientPosition     = 1 << 8,
}

interface keyable {
    [key: string]: any  
}

/*
    x64-Windows : Dumpbin.exe /exports dcmanon.dll
          1    0 00001E50 ??0DcmAnon@@QEAA@PEBD@Z = ??0DcmAnon@@QEAA@PEBD@Z (public: __cdecl DcmAnon::DcmAnon(char const *))
          2    1 000029F0 ??1DcmAnon@@QEAA@XZ = ??1DcmAnon@@QEAA@XZ (public: __cdecl DcmAnon::~DcmAnon(void))
          3    2 00002A60 ?__autoclassinit2@DcmAnon@@QEAAX_K@Z = ?__autoclassinit2@DcmAnon@@QEAAX_K@Z (public: void __cdecl DcmAnon::__autoclassinit2(unsigned __int64))
          4    3 00002170 ?getValue@DcmAnon@@QEAAPEBDAEBVDcmTagKey@@@Z = ?getValue@DcmAnon@@QEAAPEBDAEBVDcmTagKey@@@Z (public: char const * __cdecl DcmAnon::getValue(class DcmTagKey const &))
          5    4 00002000 ?getValue@DcmAnon@@QEAAPEBDW4patientInfo@@@Z = ?getValue@DcmAnon@@QEAAPEBDW4patientInfo@@@Z (public: char const * __cdecl DcmAnon::getValue(enum patientInfo))
          6    5 000026B0 ?makeAnonFile@DcmAnon@@QEAAAEAV1@HPEBD@Z = ?makeAnonFile@DcmAnon@@QEAAAEAV1@HPEBD@Z (public: class DcmAnon & __cdecl DcmAnon::makeAnonFile(int,char const *))
          7    6 00002620 ?save@DcmAnon@@QEAAAEAV1@PEBD@Z = ?save@DcmAnon@@QEAAAEAV1@PEBD@Z (public: class DcmAnon & __cdecl DcmAnon::save(char const *))
          8    7 000025B0 ?setValue@DcmAnon@@QEAAAEAV1@AEBVDcmTagKey@@PEBD@Z = ?setValue@DcmAnon@@QEAAAEAV1@AEBVDcmTagKey@@PEBD@Z (public: class DcmAnon & __cdecl DcmAnon::setValue(class DcmTagKey const &,char const *))
          9    8 000021F0 ?setValue@DcmAnon@@QEAAAEAV1@W4patientInfo@@PEBD@Z = ?setValue@DcmAnon@@QEAAAEAV1@W4patientInfo@@PEBD@Z (public: class DcmAnon & __cdecl DcmAnon::setValue(enum patientInfo,char const *))

    x64-linux : nm -g libdcmanon.so | grep DcmAnon
                00000000001392e0 W _ZN7DcmAnon12makeAnonFileEiPKc
                0000000000139216 W _ZN7DcmAnon4saveEPKc
                00000000001385b2 W _ZN7DcmAnon8getValueE11patientInfo
                0000000000138aac W _ZN7DcmAnon8setValueE11patientInfoPKc
                0000000000138446 W _ZN7DcmAnonC1EPKc
                0000000000138446 W _ZN7DcmAnonC2EPKc
                00000000001399a8 W _ZN7DcmAnonD1Ev
                00000000001399a8 W _ZN7DcmAnonD2Ev
*/

const info          = {}
const ctor          = process.platform == 'linux' ? '_ZN7DcmAnonC1EPKc' : '??0DcmAnon@@QEAA@PEBD@Z'
const getValue      = process.platform == 'linux' ? '_ZN7DcmAnon8getValueE11patientInfo' : '?getValue@DcmAnon@@QEAAPEBDW4patientInfo@@@Z';
const setValue      = process.platform == 'linux' ? '_ZN7DcmAnon8setValueE11patientInfoPKc' : '?setValue@DcmAnon@@QEAAAEAV1@W4patientInfo@@PEBD@Z'
const makeAnonFile  = process.platform == 'linux' ? '_ZN7DcmAnon12makeAnonFileEiPKc' : '?makeAnonFile@DcmAnon@@QEAAAEAV1@HPEBD@Z'
const save          = process.platform == 'linux' ? '_ZN7DcmAnon4saveEPKc' : '?save@DcmAnon@@QEAAAEAV1@PEBD@Z'
const getDcmSize    = 'getDcmSize'

info[ctor]          = ['pointer', ['pointer', 'string']]
info[getValue]      = ['string', ['pointer', 'int']]
info[setValue]      = ['pointer', ['pointer', 'int', 'string']]
info[makeAnonFile]  = ['pointer', ['pointer', 'int', 'string']]
info[save]          = ['pointer', ['pointer', 'string']]
info[getDcmSize]    = ['int', []]

const _anonlib : keyable = ffi.Library(libpath, info)


_anonlib.ctor = (ex: any, filename :string)=>{
    _anonlib[ctor](ex, iconv.encode(filename, 'euc-kr'));
}
_anonlib.getValue = (ex: any, pInfo:number) => {
    return iconv.decode(_anonlib[getValue](ex, pInfo), 'euc-kr');
}
_anonlib.setValue = (ex: any, pInfo:number, substitue: string) => {
    _anonlib[setValue](ex, pInfo, iconv.encode(substitue, 'euc-kr'));
}
_anonlib.makeAnonFile = (ex: any, pInfo : number, substitue : string) =>{
    _anonlib[makeAnonFile](ex, pInfo, iconv.encode(substitue, 'euc-kr'));
} 
_anonlib.save = _anonlib[save]


export class DcmAnon{
    _ : Buffer;
    
    constructor(filepath :string){
        fs.statSync(filepath); //throwable
        this._ = Buffer.alloc(_anonlib.getDcmSize());
        _anonlib.ctor(this._, filepath);
    }

    getValue(pInfo : number) : string{
        return _anonlib.getValue(this._, pInfo);
    }
    setValue(pInfo : Number, substitute : string) : DcmAnon {
        _anonlib.setValue(this._, pInfo, substitute);
        return this;
    }
    makeAnonFile(pInfo : number, substitute : string = "") : DcmAnon{
        _anonlib.makeAnonFile(this._, pInfo , substitute);
        return this;
    }
    save(filename : string) : DcmAnon{
        _anonlib.save(this._, filename);
        return this;
    }
}

