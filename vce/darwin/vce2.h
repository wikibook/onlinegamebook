
#ifndef VCE_VCE2_H
#define VCE_VCE2_H

#if defined (_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include "vce2config.h"
#include "vce2types.h"
#include "vce2wchar.h"
#include "vce2defs.h"

#if defined(VCE_OS_WINDOWS)

#include <windows.h>

#elif defined(VCE_OS_UNIX)

#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

#endif

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <stddef.h>
#include <cstring>

namespace vce
{

	//! @ja �o�[�W�����ԍ� @endja
    extern const char *version_number;
	//! @ja �r���h�ԍ� @endja
    extern const char *build_number;

	class DefaultAllocator
		:public MemoryAllocator
	{
		void *alloc(size_t sz);
		void free(void *p);
	};

	extern DefaultAllocator defaultallocator;
	extern MemoryAllocator *globalallocator;

	template <class T>struct GlobalAltr
	{
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		template <class U>struct rebind{typedef GlobalAltr<U> other;};
		GlobalAltr()throw(){}
		GlobalAltr(const GlobalAltr&)throw(){}
		template <class U> GlobalAltr(const GlobalAltr<U>&)throw(){}
		~GlobalAltr()throw(){}
		pointer allocate(size_type num){return (pointer)globalallocator->alloc(num*sizeof(T));}
		pointer allocate(size_type num,std::allocator<void>::const_pointer){return allocate(num);}
		void construct(pointer p,const T& value){new((void*)p)T(value);}
		void deallocate(pointer p){globalallocator->free(p);}
		void deallocate(pointer p,size_type){deallocate(p);}
		void destroy(pointer p){p->T::~T();}
		pointer address(reference value){return &value;}
		const_pointer address(const_reference value)const{return &value;}
		size_type max_size() const throw(){return 0xffffffff/sizeof(T);}
		void operator=(const GlobalAltr<T>&)throw(){}
	};

	template <class T>struct Altr
	{
		MemoryAllocator *mm;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		template <class U>struct rebind{typedef Altr<U> other;};
		explicit Altr(MemoryAllocator *&ma)throw():mm(ma){}
		Altr(const Altr&a)throw():mm(a.mm){}
		template <class U> Altr(const Altr<U>&a)throw():mm(a.mm){}
		~Altr()throw(){}
		pointer allocate(size_type num){return (pointer)mm->alloc(num*sizeof(T));}
		pointer allocate(size_type num,std::allocator<void>::const_pointer){return allocate(num);}
		void construct(pointer p,const T& value){new((void*)p)T(value);}
		void deallocate(pointer p){mm->free(p);}
		void deallocate(pointer p,size_type){deallocate(p);}
		void destroy(pointer p){p->T::~T();}
		pointer address(reference value){return &value;}
		const_pointer address(const_reference value)const{return &value;}
		size_type max_size() const throw(){return 0xffffffff/sizeof(T);}
		void operator=(const Altr<T>&a)throw(){this->mm=a.mm;}
	};

	template <class T1, class T2>bool operator==(const Altr<T1>&, const Altr<T2>&) throw() { return true; }
	template <class T1, class T2>bool operator!=(const Altr<T1>&, const Altr<T2>&) throw() { return false; }
	template <class T1, class T2>bool operator==(const GlobalAltr<T1>&, const GlobalAltr<T2>&) throw() { return true; }
	template <class T1, class T2>bool operator!=(const GlobalAltr<T1>&, const GlobalAltr<T2>&) throw() { return false; }

	template<typename T>Altr<T>Altring(MemoryAllocator *&ma){Altr<T> a(ma);return a;}

	template<typename T>
	struct l_vector
		:public std::vector<T,Altr<T> >
	{
		l_vector(MemoryAllocator *&ma)
			:std::vector<T,Altr<T> >(Altring<T>(ma))
		{
		}
	};

	template<typename T>
	struct l_basic_string
		:public std::basic_string<T,std::char_traits<T>,Altr<T> >
	{
		l_basic_string(MemoryAllocator *&ma)
			:std::basic_string<T,std::char_traits<T>,Altr<T> >(Altring<T>(ma))
		{
		}
	};

	typedef l_basic_string<char> l_string;


	template<typename A,typename B>
	struct l_map
		:public std::map<A,B,std::less<A>,Altr<std::pair<A,B> > >
	{
		MemoryAllocator *&localallocator;
		typedef std::map<A,B,std::less<A>,Altr<std::pair<A,B> > > Super;

		explicit l_map(MemoryAllocator *&ma)
			:localallocator(ma)
			,Super(std::less<A>(),Altring<std::pair<A,B> >(ma))
		{
		}
		template<typename AA,typename AB>
		l_map(MemoryAllocator *&ma,const std::map<AA,AB> &copy)
			:localallocator(ma)
			,Super(std::less<A>(),Altring<std::pair<A,B> >(ma))
		{
			typename Super::iterator to=Super::begin();
			for(typename std::map<AA,AB>::const_iterator from=copy.begin();from!=copy.end();from++)
			{
				to=Super::insert(to,typename Super::value_type(A(localallocator,from->first),B(localallocator,from->second)));
				to++;
			}
		}
		template<typename AA,typename AB>
		void operator=(const std::map<AA,AB> &copy)
		{
			Super::erase();
			typename Super::iterator to=Super::begin();
			for(typename std::map<AA,AB>::const_iterator from=copy.begin();from!=copy.end();from++)
			{
				to=Super::insert(to,typename Super::value_type(A(localallocator,from->first),B(localallocator,from->second)));
				to++;
			}
		}
		template<typename T>
		B&operator[](const T &key)
		{
			return this->operator[](A(localallocator,key));
		}
		B&operator[](const A &key)
		{
			typename l_map<A,B>::iterator i=lower_bound(key);
			if(i==Super::end())
				i=this->insert(i,typename Super::value_type(key,B(localallocator)));
			return ((*i).second);
		}
		void operator=(const l_map &copy)
		{
			insert(copy.begin(),copy.end());
		}
	};


	template <typename A,typename B>struct g_map:public std::map<A,B,std::less<A>,GlobalAltr<std::pair<A,B> > >{};

	template<typename T>struct g_vector:public std::vector<T,GlobalAltr<T> >{};

	template<typename T>struct g_basic_string:public std::basic_string<T,std::char_traits<T>,GlobalAltr<T> >
	{
		typedef std::basic_string<T,std::char_traits<T>,GlobalAltr<T> > Super;
		g_basic_string():Super(){}
		g_basic_string(const T*c):Super(c){}
		template<typename Any>
		g_basic_string(const std::basic_string<T,std::char_traits<T>,Any> &c):Super(c.c_str()){}
		template<typename Any>
		operator std::basic_string<T,std::char_traits<T>,Any>()
		{
			return Super::c_str();
		}
	};
	typedef g_basic_string<char> g_string;
	typedef g_basic_string<vce::wchar> g_wstring;

	template<typename T>struct g_basic_stringstream:public std::basic_stringstream<T,std::char_traits<T>,GlobalAltr<T> >{};
	typedef g_basic_stringstream<char> g_stringstream;

	//!@ja �N���e�B�J���łȂ���O�̎�� @endja
    enum VCE_EXCEPTION
    {
		//!@ja �s�� @endja
        VCE_EXCEPT_UNKNOWN=0,
		//!@ja Socket�쐬���s @endja
        VCE_EXCEPT_CREATESOCKETFAIL,
		//!@ja �|�[�g���蓖�Ď��s @endja
        VCE_EXCEPT_BINDFAIL,
		//!@ja Listen���s @endja
        VCE_EXCEPT_LISTENINGFAIL,
		//!@ja �m���u���b�L���O�����s @endja
		VCE_EXCEPT_NONBLOCKING,
		//!@ja �ڑ����s @endja
        VCE_EXCEPT_CONNECTFAIL,
		//!@ja �ڑ����Ԑ؂� @endja
		VCE_EXCEPT_CONNECTTIMEOUT,
		//!@ja �ڑ�����������Ă��� @endja
		VCE_EXCEPT_CONNECTREFUSED,
		//!@ja ���O�������s @endja
        VCE_EXCEPT_NAMERESOLUTIONFAIL,
		//!@ja �Í������������s @endja
        VCE_EXCEPT_KEYEXCHANGEFAIL,
		//!@ja ��M�o�b�t�@�I�[�o�[ @endja
        VCE_EXCEPT_RECVBUFFER_FULL,
		//!@ja ���M�o�b�t�@�I�[�o�[ @endja
        VCE_EXCEPT_SENDBUFFER_FULL,
		//!@ja SIGPIPE @endja
		VCE_EXCEPT_SIGPIPE,
		//!@ja �s���f�[�^ @endja
        VCE_EXCEPT_INVALIDDATA,
		//!@ja �Z�b�V�����^�C���A�E�g @endja
		VCE_EXCEPT_TIMEOUT,
		//!@ja �\�P�b�g��t�@�C���n���h���̉ߑ� @endja
		VCE_EXCEPT_TOOMANYFILES,
		//!@ja ���łɎg���Ă���|�[�g�ԍ� @endja
		VCE_EXCEPT_ALREADYLISTEN,
		//!@ja �R�l�N�V�������Z�b�g @endja
		VCE_EXCEPT_CONNECTIONRESET,
		//!@ja ���M�L���[��� @endja
		VCE_EXCEPT_OVERFLOWSENDQUEUE,
		//!@ja ���M�o�b�t�@���@@endja
		VCE_EXCEPT_OVERFLOWSENDBUFFER
    };

	//!@ja �Z�b�V�����⃊�X�i�[�̏�� @endja
    enum VCE_STATE
    {
		//!@ja �s�� @endja
        VCE_STATE_UNKNOWN=0,
		//!@ja �ڑ��� @endja
        VCE_STATE_CONNECTING,
		//!@ja �Z�b�V�������쐬����Ă���ڑ�����܂ŁA����ѐڑ����������Z�b�V�������L���ɂȂ�܂� @endja
        VCE_STATE_PREPROCESS,
		//!@ja �Z�b�V�������L���̂Ƃ� @endja
        VCE_STATE_ESTABLISHED,
		//!@ja ���X�i�[�����X�j���O��Ԃ̂Ƃ� @endja
        VCE_STATE_LISTENING,
		//!@ja ����ꂽ��B���X�i�[��Z�b�V�����͕�����ƍ폜����邽�߁A���ۂ� CLOSED �ɂȂ鎞�Ԃ͔��ɒZ���A���̌� UNKNOWN �Ɉڍs���܂��B @endja
        VCE_STATE_CLOSED
    };

	//!@ja �ؒf���ꂽ�Ƃ��̌����̎�� @endja
    enum VCE_CLOSEREASON
    {
		//!@ja �s�� @endja
        VCE_CLOSE_UNKNOWN=0,
		//!@ja �ؒf���� @endja
        VCE_CLOSE_LOCAL,
		//!@ja �ؒf���ꂽ @endja
        VCE_CLOSE_REMOTE,
		//!@ja ��M�o�b�t�@�t�� @endja
        VCE_CLOSE_RECVBUFFERFULL,
		//!@ja ��M�G���[ @endja
        VCE_CLOSE_RECVERROR,
		//!@ja �s���f�[�^��M @endja
        VCE_CLOSE_INVALIDDATA
    };

	//!@ja ���O���x���t���O @endja
	enum VCE_LOGLEVEL
	{
		//!@ja ���Ȃ߁A�d�v�ȃ��O @endja
		VCE_LOGLEVEL_LOW=1,
		//!@ja ��⑽�߁AVCE�̓�������̃��O @endja
		VCE_LOGLEVEL_MEDIUM=2,
		//!@ja ���߁AOS���x����API�Ăяo���̃��O @endja
		VCE_LOGLEVEL_HIGH=4
	};

    class Base;
    class Session;
    class Listener;
	class Finder;
	class MonitorListener;
	typedef std::vector<MonitorListener*> MonitorListenerList;

	static const size_t VCE_BUFFER_ERROR=(size_t)-1;

    //!@ja  VCE���C���N���X @endja
    /*!@ja 
    VCE�I�u�W�F�N�g�͂��̃N���X���璼�ڐ��������AVCECreate�֐��̕Ԃ�l���g���悤�ɂ��Ă��������B
    �g���I����č폜����Ƃ���VCEDelete�֐��ɓn���܂��B
    �����������邱�Ƃ��ł��܂��B
    @endja
    */

    class VCE//abstract
    {
	protected:
		VCE();
		virtual ~VCE();
    public:
		//!@ja �A���P�[�^ @endja
		vce::MemoryAllocator *ma;

        //!@ja VCE�̃��b�Z�[�W�|�[�����O @endja
        /*!@ja 
        ����I�ɌĂяo�����Ƃɂ��VCE�������b�Z�[�W�����o���܂��B
        ���̃��\�b�h���Ăяo�����X���b�h����e�N���X�̃��[�U��`���z�֐����Ăяo����܂��B
        @endja
        */
        virtual void Poll()=0;

        //!@ja VCE �̃��b�Z�[�W�|�[�����O���� Wait @endja
        /*!@ja 
		Poll ��A���ŌĂяo�����Ƃ��� CPU ���g���؂�Ȃ��悤�ɂ��܂��B
		�Ⴆ�� 10 �~���b�Ɛݒ肵���ꍇ�́A 1 �b�Ԃ� 100 ����x Poll ���Ă΂��悤�� Sleep ���܂��B���ׂ������A100 ��������Ԋu�̂Ƃ��� Sleep ���܂���B
		Sleep �̌덷�ɂ�萳�m�ȌĂяo���񐔂ɂȂ�Ȃ����Ƃ�����܂��B
        @param wait �~���b�P�ʂŁA wait ���ԓ��ɍēx Poll ���Ă΂ꂽ�Ƃ��� Sleep ���܂��B�f�t�H���g�� 0 �ł��B
		@endja
		*/
        virtual void SetPollingWait(int wait=10)=0;

		//!@ja 1���Poll�ŏ������郊�X�i�[�ƃZ�b�V�����̐��𐧌����܂� @endja
		/*!@ja
		1���Poll�ŏ������郊�X�i�[�ƃZ�b�V�����̐��𐧌����܂��B
		�Z�b�V�������������Ȃ�ق�Poll�̌Ăяo�����珈�����߂�܂łɎ��Ԃ�������悤�ɂȂ�܂��B
		1���Poll�ŏ������郊�X�i�[�ƃZ�b�V�����̐��𒲐����邱�ƂŁA�����𕪎U�ł��܂��B
		�����������Poll���珈�����߂�̂������Ȃ�܂����A�ʐM�̃��X�|���X�������Ȃ�܂��B
		�傫�������Poll���珈�����߂�̂Ɏ��Ԃ�������܂����A�ʐM�̃��X�|���X���ǂ��Ȃ�܂��B
		�f�t�H���g��64�ɂȂ��Ă���AWindows�ł�64�ȉ��ɐ�������܂��B
		@param limit �����l
		@endja
		*/
		virtual void SetSelectLimit(unsigned int limit)=0;


        //!@ja �ڑ����܂��B @endja
        /*!@ja 
        �Z�b�V����s���g��hostname:port�ɐڑ����܂��B
        @param s Session�h���N���X�ւ̃|�C���^��n���܂��B�n�����|�C���^�͓����ŊǗ�����A�����Ŏg�p����Ȃ��Ȃ�Ɛ؂藣����܂��B���Ɏg���Ă���I�u�W�F�N�g��ݒ肷�邱�Ƃ͂ł��܂���B
        @param hostname �ڑ���z�X�g����������IP�A�h���X���L�q����������B
        @param port �ڑ���TCP�|�[�g�ԍ��B
        @param timeout �ڑ������݂�~���b���ԁB������߂��Ă��Ȃ���Ȃ��ꍇ��Exception�֐����R�[������܂��B0���w�肵���ꍇ�́A�Ȃ��邩�L�����Z�������܂Ŏ��s���܂��B
        @return ���j�[�NID�A���s�����ꍇ��0
        @sa Session
		@sa Session::CancelConnecting
		@sa Listen
        @endja
        */
		virtual VUint32 Connect(Session *s,const std::string& hostname,VUint16 port,unsigned int timeout=0)=0;
        virtual VUint32 Connect(Session *s,const vwstring& hostname,VUint16 port,unsigned int timeout=0)=0;
		//!@ja UDP�Őڑ����܂��B @endja
		/*!@ja
		TCP�̑����UDP�Őڑ����܂��B�����UdpListen���Ă���T�[�o�ł��B�|�[�g�ԍ���UDP�|�[�g���w�肵�܂��B���̑��̎g������Connect�֐��Ɠ����ɂȂ�܂��B
		@sa Connect
		@sa UdpListen
		@endja
		*/
		virtual VUint32 UdpConnect(Session *s,const std::string& hostname,VUint16 port,unsigned int timeout=0)=0;
		virtual VUint32 UdpConnect(Session *s,const vwstring& hostname,VUint16 port,unsigned int timeout=0)=0;

		//!@ja LoopbackListener�ɐڑ����� @endja
		/*!@ja
        ����VCE�I�u�W�F�N�g��LoopbackListen���Ă���|�[�g�ԍ����w�肵�A�ڑ����܂��B
		@param port �|�[�g�ԍ��B
		@return �Z�b�V������ID�B���s�����0���Ԃ�܂��B
		@endja
		*/
		virtual VUint32 LoopbackConnect(Session *s,VUint16 port)=0;

		//!@ja �ڑ���҂��󂯂܂��B @endja
        /*!@ja 
        port�����X�j���O(�҂�����)���܂��B
        @param l ���X�j���O�Ɏg�����X�i�[�I�u�W�F�N�g�ւ̃|�C���^��n���܂��B�n�����|�C���^�͓����ŊǗ�����A�g�p����Ȃ��Ȃ�Ɛ؂藣����܂��B���Ɏg���Ă���I�u�W�F�N�g��ݒ肷�邱�Ƃ͂ł��܂���B
        @param port ���X�j���O����TCP�|�[�g�ԍ����w�肵�܂��B
        @param bindaddress ���X�j���O����IP�A�h���X���w�肵�܂��BNULL�Ȃ玩���̎��S�A�h���X�ɂȂ�܂��B
        @return ���j�[�NID
        @endja
        */
		virtual VUint32 Listen(Listener *l,VUint16 port,const std::string& bindaddress="")=0;

        //!@ja UDP�ڑ���҂��󂯂܂��B @endja
		/*!@ja
		TCP�̑����UDP�|�[�g�ő҂��󂯂܂��B�g������Listen�֐��Ɠ����ɂȂ�܂��B
		@sa Listen
		@sa UdpConnect
		@endja
		*/
		virtual VUint32 UdpListen(Listener *l,VUint16 port,const std::string& bindaddress="")=0;

		//!@ja Proxy�ڑ��p��Listen @endja
		/*!@ja
		ProxyListen�֐���Proxy�v���O�����ɐڑ����AProxy�v���O�����ɑ΂���Listen�v�����o���܂��B
		ProxyListen�֐����g���Ă������ɐڑ��\�ȏ�Ԃɂ͂Ȃ炸�AProxy�v���O�����̏������ł��Ă���
		���߂Đڑ��\�ȏ�ԂɂȂ�܂��B
		Proxy�v���O�������I��������AProxy�v���O�����Ƃ̐ڑ����؂ꂽ�肷��ƃ��X�i�[�������܂��B
		@param l ���X�i�[�I�u�W�F�N�g
		@param proxyport Proxy�v���O������backendport
		@param proxyaddress Proxy�v���O���������s���Ă���PC�̃A�h���X
		@param password Proxy�v���O�����ɐڑ����邽�߂̃p�X���[�h�B�ݒ肵�Ă��Ȃ��ꍇ�͏ȗ��ł��܂�
		@endja
		*/
		virtual VUint32 ProxyListen(Listener *l,VUint16 proxyport,const std::string& proxyaddress,const std::string& password="")=0;

		//!@ja Loopback Listen @endja
		/*!@ja
		����VCE�I�u�W�F�N�g���ł̂ݗ��p�\�ł��B
		@param port ���X�j���O����|�[�g�ԍ��BTCP��UDP�̃|�[�g�Ƃ͕ʂɂȂ�܂��B
		@return ���������ꍇ�̓��X�i�[��ID�B���s�����0���Ԃ�܂��B
		@endja
		*/
		virtual VUint32 LoopbackListen(Listener *l,VUint16 port)=0;
		
		//!@ja Finder��ݒ肵�܂� @endja
		/*!@ja
		@param f �ݒ肵����Finder�C���^�[�t�F�C�X�ւ̃|�C���^�B
		@param port Finder�Ɏg��UDP�|�[�g�ԍ��B
		@param address �o�C���h����A�h���X�B
		@param range port���g���Ȃ������Ƃ���port+range���ōĎ��s���܂��B
		@endja
		*/
		virtual vce::VUint32 CreateFinder(vce::Finder *f,vce::VUint16 port=59820,const std::string& bindaddress="",vce::VUint16 range=10)=0;

        //!@ja �C���^�[�t�F�C�X�I�u�W�F�N�g���擾���܂��B @endja
        /*!@ja 
        @param uID �Z�b�V�����܂��̓��X�i�[��uID�B
        @return �L���ȃZ�b�V�����܂��̓��X�i�[�A�����ȏꍇ��NULL�B
        @endja
        */
		virtual Session* GetSession(VUint32 uID)=0;
		virtual Listener* GetListener(VUint32 uID)=0;

		//!@ja���ׂẴZ�b�V�������擾���܂��B @endja
		/*!@ja
		���ׂẴZ�b�V�������擾���܂��B
		@param v �Z�b�V�������m�ۂ��邽�߂�Session*��vector�B�����I�ɃZ�b�V��������resize����܂��B
		@endja
		*/
		virtual void GetAllSession(std::vector<Session*> &v)=0;

		//!@ja Listen����Ƃ��̃A�h���X/�|�[�g���ė��p���܂��B @endja
		/*!@ja
		Windows�̏ꍇ�A�ė��p����悤�ݒ肷��ƁA�T�[�o�̃|�[�g�����炩�̕s��ɂ������Ă��Ȃ��Ƃ��ł��A
		Listen�ɂ͐�������悤�ɂȂ�܂��B�����AOS�����̃|�[�g�������悤�ɂȂ�܂Őڑ����󂯕t���邱�Ƃ��ł��܂���B
		�܂��A���łɂ��̃|�[�g���g���Ă��Ă����s�ƂȂ�܂���B
		Unix�nOS�̏ꍇ�A��������̂̓��X�i�[�������Ă���Ƃ��݂̂ł��B���X�i�[�̔h���Z�b�V�������c���Ă��Ă����܂��܂���B
		���łɃ��X�j���O��Ԃ̃A�h���X/�|�[�g�ւ̃o�C���h�͎��s���܂��B
		@param use �ė��p����Ȃ�true�B�����l��false�B
		@sa Listen
		@endja
		*/
		virtual void ReUseAddress(bool use)=0;

		//!@ja TCP��Listen����Ƃ���Accept�҂�����ݒ肵�܂��B @endja
		/*!@ja
		TCP��Listen����Ƃ���Accept�҂�����ݒ肵�܂��B�Z�����Ԃő����̐ڑ���������A
		Connect����Ă���Ƃ���Poll�̌Ăяo���Ԋu���x�������肷���Accept�҂��������܂��Ă��܂��܂��B
		�����ɐڑ����悤�Ƃ����Ƃ��ɐڑ����s���Ă��܂��Ƃ��͂��̊֐���
		�҂����𑝂₵�Ă݂Ă��������B�����l��100�ł��B
		���̊֐���Listen����O�ɌĂяo���K�v������܂��B
		Unix�nOS�ł́AOS�ɂ��ő吔�̐���������܂��BCentOS�ł�/proc/sys/net/core/somaxconn�̒l�ɐ�������܂��B
		Solaris�ł�/usr/sbin/ndd -set /dev/tcp tcp_conn_req_max_q��/usr/sbin/ndd -set /dev/tcp tcp_conn_req_max_q0�R�}���h�ɂ�萧����ύX�ł��܂��B
		Windows�ł͂��̒l�͌��͂������܂���B
		@param backlog Accept�҂���
		@sa Listen
		@endja
		*/
		virtual void SetTCPListenBacklog(unsigned int backlog)=0;

        //!@ja �g�p���̃��X�i�[�ƃZ�b�V�����̐���Ԃ��܂��B @endja
        virtual unsigned int GetUsed()=0;

		//!@ja �C���^�[�t�F�C�X��VCE���g�p�������ׂ܂��B @endja
		/*!@ja
		Connect�����Z�b�V������Listen�������X�i�[���܂��g���Ă��邩�𒲂ׂ邱�Ƃ��ł��܂��B�g���Ă��Ȃ��ꍇ�͍ė��p���邱�Ƃ�Adelete���邱�Ƃ��ł��܂��B
		@param iface ���ׂ�Ώۂ̃C���^�[�t�F�C�X
		@return true�̏ꍇ�͎g�p���ł�
		@endja
		*/
		virtual bool IsUsing(Base *iface)=0;


        //!@ja ���O�o�͐�֐���ݒ肵�܂� @endja
		/*!@ja
		SetLogger �Ƃ̕��p�͂ł��܂���B
		callback �� NULL �ȊO�ɐݒ肷��� ostream ��肱���炪�D�悳��܂��B
		@endja
		*/
		virtual void SetLogCallback(void(*callback)(const char *log,int level))=0;

        //!@ja ���O�o�͐�X�g���[����ݒ肵�܂� @endja
        /*!@ja 
		�o�͐�ƃ��x�����w�肵�܂��B���ׂẴ��O������ɂ́A���ꂼ��̃��x���ŏo�͐��ݒ肵�Ȃ���΂Ȃ�܂���B
		���x�����ɕʁX�̏o�͂ɂ��邱�Ƃ��ł��܂��B
		�o�͐�͂��ꂼ��̃��x���ɂ� 1 �݂̂ŁA���� VCE �I�u�W�F�N�g�Ƃ����L����܂��B
		SetLogCallback �Ƃ̕��p�͂ł��܂���B
		@param target �o�͐�̃X�g���[���ւ̃|�C���^�B
		@param level �ݒ肷�郌�x���t���O�̑g�ݍ��킹�B
		@sa VCE_LOGLEVEL
        @endja
        */
        virtual void SetLogger(std::ostream *target,int level=VCE_LOGLEVEL_LOW)=0;

		//!@ja ���O�̏o�͐�X�g���[�����擾���܂� @endja
		/*!@ja
		���O�̏o�͐�X�g���[�����擾���܂��B
		@param level �o�͐�X�g���[�������o�����O���x���B
		@return ���o�������O�o�͐�X�g���[���B�ݒ肳��Ă��Ȃ��ꍇ��0�ł��B
		@endja
		*/
        virtual std::ostream *Log(int level=0)=0;

		virtual void GetMonitorListener(MonitorListenerList &ml)=0;

		//!@ja �I�u�W�F�N�g�����ׂď��������̂�҂��܂� @endja
		/*!@ja
		���̊֐����g���ƁAConnect��Listen�����I�u�W�F�N�g�����ׂĕ�����܂�Poll���đ҂̂ŁA
		���S��VCEDelete�ł���悤�ɂȂ�܂��B
		@param timeout ������҂~���b���ԁB
		@endja
		*/
		virtual void SafeDelete(vce::VUint64 timeout)=0;

		//!@ja �ꎞ�I�ȃo�b�t�@���m�ۂ��܂��B @endja
		/*!@ja
		size �Ŏw�肵���o�C�g���̃o�b�t�@���m�ۂ��܂��B����Ȃ��Ƃ��͊g������܂��B
		VCEDelete�̂Ƃ��ɂ��ׂĉ������܂��B
		���gen�̃f�[�^�\�z���ɁA�X�^�b�N�̑���Ɏg���܂��B
		@param size �m�ۂ���o�C�g��
		@return �m�ۂ��ꂽ�o�b�t�@
		@endja
		*/
		virtual VUint8 *BufferPush(std::size_t size)=0;
        
		//!@ja �Ō�Ɋm�ۂ����o�b�t�@��������܂��B @endja
		/*!@ja
		���ۂɂ͉�������ɁA����Push���ꂽ�Ƃ��ɂ��̂܂܎g����悤�Ɏc���܂��B
		@endja
		*/
		virtual void BufferPop()=0;

		//!@ja ������BufferPop���R�[������N���X�ł��B @endja
		class AutoPop
		{
			VCE *api;
		public:
			AutoPop(VCE *api);
			~AutoPop();
		};
		//!@ja �m�ۂ��ꂽ�o�b�t�@�̍��v�e�ʂ��擾���܂��B @endja
		/*!@ja
		�g�p����ĂȂ��o�b�t�@���܂߂āA�m�ۂ���Ă���o�b�t�@�̑��e�ʂ��擾���܂��B
		@endja
		*/
		virtual size_t BufferSize()=0;

		//!@ja Think �֐��̃R�[���o�b�N�p�x��ݒ肵�܂� @endja
		/*!@ja
		Session �� Listener �̃����o�֐��ł��� Think �֐��̃R�[���o�b�N�p�x��ݒ肵�܂��B
		Poll ���Ƀ`�F�b�N����A�O��� Poll ���� interval �~���b���߂��Ă����� Think ���R�[�����܂��B
		����������� Think ���Ă΂ꂷ���Ė��ʂ��傫���Ȃ�܂��B�t�ɑ傫������� Codec �N���X��
		timeout �̔������݂����肵�܂��B�܂� Poll �֐��̌Ăяo���Ɉˑ�����̂ŁA�Ⴆ�� 10 ��ݒ肵�Ă� 1 �b�ԂɕK�� 100 ��Ă΂��킯�ł͂���܂���B
		@param interval �C���^�[�o���~���b�B�f�t�H���g�� 10 �ł��B
		@endja
		*/
		virtual void SetThinkIntervalTime(VUint32 interval)=0;
		//!@ja �Í������p���̌������̃^�C���A�E�g��ݒ肵�܂��B @endja
		/*!@ja
		@param timeout �^�C���A�E�g�~���b�B�f�t�H���g�� 5000 �ł��B
		@endja
		*/
		virtual void SetKeyExchangeTimeoutTime(VUint32 timeout)=0;
	private:
		VCE(const VCE&);
		VCE& operator=(VCE&);
	};

    //Utility
    //!@ja 16�i�̒���2�̕�����𐔒l�ɂ��܂��B @endja
    /*!@ja 
    "10"��16 "aa"��170 "FF"��255 �̂悤�ɕϊ�����܂��B�}�C�i�X��ꌅ��16�i�ɂ͑Ή����Ă��܂���B
	@param XX 16�i���l�̕�����
	@return 10�i���̐��l�B���s�����Ƃ���-1�B
    @endja
    */
	int XXtoi(const vce::g_string& _XX);

    //selfIP(s)
	//!@ja �����̃A�h���X���擾���܂� @endja
	/*!@ja 
	Windows�̏ꍇ�A�A�h���X���擾�ł���̂�VCECreate���Ă񂾌�ɂȂ�܂��B
	���[�J�����[�v�o�b�N���܂߂ĕ����̃f�o�C�X�����݂���Ƃ��͕����̃A�h���X���擾�ł��܂��B
	array_qt �𒴂��鐔�̃A�h���X�����������ꍇ�� iparray �ɂ͒��������͏������܂ꂸ�A�K�v�Ȑ����Ԃ�܂��B
	@param iparray �A�h���X���󂯎�邽�߂̔z��
	@param array_qt �z��̍ő�v�f��
	@return �擾�����A�h���X�̐�
	@endja
	*/
    size_t GetSelfAddress(unsigned int *iparray,size_t array_qt);


    //!@ja VUint32�ȃA�h���X�𕶎���ɕϊ� @endja
    /*!@ja 
    IPv4�A�h���X�𕶎���ɕϊ����܂��B
    @param ip �A�h���X
    @return ������
	@sa StringToAddr
    @endja
    */
	vce::g_string AddrToString(const VUint32 &ip);
    //!@ja IP�A�h���X�𐔒l�ɕϊ����܂��B @endja
    /*!@ja 
    @param ip "192.168.1.2"�̂悤�ȃA�h���X������
    @return �������Ȃ��Ƃ�0xffffffff
	@sa AddrToString
    @endja
    */
	VUint32 StringToAddr(const vce::g_string& _ip);

    //!@ja �G���f�B�A���ϊ� @endja
    /*!@ja 
    bin�̃G���f�B�A���i�o�C�g�I�[�_�[�j��ϊ����܂��B
    @param bin �ϊ�����32�r�b�g�l
    @return �ϊ����ꂽ32�r�b�g�l
    @endja
    */
    VUint32 bswap(VUint32 bin);
    //!@ja �G���f�B�A���ϊ� @endja
    /*!@ja 
    bin�̃G���f�B�A���i�o�C�g�I�[�_�[�j��ϊ����܂��B
    @param bin �ϊ�����16�r�b�g�l
    @return �ϊ����ꂽ16�r�b�g�l
    @endja
    */
    VUint16 xchg(VUint16 bin);

    //!@ja ���������� @endja
    /*!@ja 
    sz_p�̑傫�������������̈�p�̒��ŁAsz_cmp�̑傫����������������cmp���ŏ��Ɍ����ʒu�������܂��B
    @param p ��������郁�����ʒu�ւ̐擪�|�C���^
    @param sz_p ��������郁�����T�C�Y
    @param cmp �������郁�����ʒu�ւ̐擪�|�C���^
    @param sz_cmp �������郁�����T�C�Y
    @return ���������ꍇ�͌��������ꏊ�ւ̐擪�|�C���^�B����ȊO�̏ꍇ��NULL�B
    @endja
    */
    void *memmem(const void *p,size_t sz_p,const void *cmp,size_t sz_cmp);
    //!@ja �����񂩂琔�l�ɕϊ� @endja
    /*!@ja 
    ������𐔎��ɕϊ�����W���֐���atoi�֐��̗����ŁB
    int�͈̔�(-2147483648����2147483647)�܂ł̕ϊ����ł��܂��B�����̑O�Ɍ��ꂽ�����͖�������܂��B
    @param str �ϊ����镶����
    @return ���l�A����ɕϊ��ł��Ȃ������ꍇ��0
    @endja
    */
	int atoi(const vce::g_string& _str);

	//!@ja ������ɕϊ� @endja
	template<typename from>
	vce::g_string ntoa(from n)
	{
		vce::g_stringstream ss;
		ss<<n;
		return ss.str();
	}

	//!Calendar time
	struct CalendarTime
	{
		//!@ja ����N @endja
		int year;
		//!@ja 1-12 @endja
		int month;
		//!@ja 1- @endja
		int day;
		//!@ja 1-24 @endja
		int hour;
		//!@ja 0-59 @endja
		int minute;
		int second;
		int milliseconds;
	};

    //!@ja ���Ԏ擾 @endja
    /*!@ja 
    ��ԍŏ��ɌĂяo���ꂽ�Ƃ�����̃~���b�P�ʂ̎��Ԃ��擾���܂��B
    @return ����
    @endja
    */
    VUint64 GetTime();

    //!@ja �J�����_�[�`���̎��Ԏ擾 @endja
    /*!@ja 
    �J�����_�[�`���̎��Ԃ��擾���܂��B�߂�l��CalendarTime�\���̂ɔN����~���b�܂ł̏�񂪓���܂��B
    @return ����
    @endja
    */
	CalendarTime GetCalendarTime();

    //!@ja �X���b�h��Sleep @endja
    /*!@ja 
    �~���b�P��(���x�͍r��)�ŃX���b�h���ꎞ��~���܂��B
    @param millsec �~�߂鎞��
    @endja
    */
    void ThreadSleep(VUint32 millsec);

    //!@ja ������̃o�C�g�� @endja
    /*!@ja
    strlen�̃o�b�t�@�T�C�Y�w��ŁB�ʏ��strlen�ł�NULL�I�[����ĂȂ��ꍇ�o�b�t�@�I�[�o�[�������Ă��܂��A
	���ׂ�o�b�t�@�T�C�Y�ɐ��������������Ƃ���strlen_s���g���܂��B
    @endja
    */
    size_t strlen_s(const char *str,size_t buflen);
    size_t strlen(const char *str);
    //!@ja ��������R�s�[ @endja
    /*!@ja 
    strcpy�̃o�b�t�@�T�C�Y�����ŁB�R�s�[���ƃR�s�[��̃o�b�t�@�T�C�Y���w��ł��܂��B
    @endja
    */
    void strncpy(char *dest,size_t destlen,const char *src,size_t srclen);
    //!@ja ��������R�s�[ @endja
    /*!@ja 
    strcpy�̃o�b�t�@�T�C�Y�����ŁB�R�s�[��̃o�b�t�@�T�C�Y���w��ł��܂��B
    @endja
    */
    void strcpy_s(char *dest,size_t destlen,const char *src);
    //!@ja UTF8������̕����� @endja
    /*!@ja 
    UTF8��1������1�o�C�g�ȏ�g���̂ŁA�������𐔂���Ƃ��͂��̊֐����g���܂��B
    @param utf8string UTF8������
    @param buflen �o�b�t�@�T�C�Y
    @return ������
    @endja
    */
    size_t utfstrnlen(const utf8 *utf8string,size_t buflen);
    //!@ja UTF16�̕������𒲂ׂ� @endja
    /*!@ja 
    UTF16�̕������𒲂ׂ܂��B�T���Q�[�g�y�A�ɑΉ����Ă��܂���B
    @endja
    */
    size_t wcslen(const wchar *str);
    //!@ja UTF16��UTF8 @endja
    /*!@ja 
    UTF16����UTF8�ɕϊ����܂��B
    @param dest �o�͐��char�z��
    @param destlen �o�͐�̃o�b�t�@�T�C�Y
    @param src UTF16�̕�����
    @endja
    */
    size_t utf16to8(utf8 *dest,size_t destlen,const wchar *src);
	 //!@ja UTF16��UTF8 @endja
    /*!@ja 
    UTF16��UTF8�̕ϊ������܂��B
    @param wstr UTF16�̕�����
	@return UTF8�̕�����
    @endja
    */
	std::string utf16to8(const vwstring &wstr);
    //!@ja UTF8��UTF16 @endja
    /*!@ja UTF8��UTF16�̕ϊ������܂��B
    @param dest �o�͐��wchar�z��
    @param destlen �o�͐�̃o�b�t�@�̃o�C�g��
    @param src UTF8�̕�����
    @endja
    */
    size_t utf8to16(wchar *dest,size_t destlen,const utf8 *src);
    //!@ja UTF8��UTF16 @endja
    /*!@ja UTF8��UTF16�̕ϊ������܂��B
    @param str UTF8�̕�����
	@return UTF16�̕�����
    @endja
    */
	vwstring utf8to16(const std::string& str);
    //STL�g��Utl
	typedef std::map<std::string,std::string> ConfigMap;
    //!@ja �ݒ�t�@�C����ǂݍ��� @endja
    /*!@ja 
    name=value �`���̕������conf["name"]==value�Ƀ}�b�v���܂��B"#"�L���ȍ~�ƁA"#"�L���̑O�̋󔒁A"="�̑O��̋󔒂͖�������܂��B
    @param filename �ݒ�t�@�C��
    @endja
    */
	ConfigMap LoadSimpleConfig(const std::string& filename);

    /*!internal
	 * 
	 */
	ConfigMap LoadSimpleConfig(std::istream& input);

    //!@ja ���Ԃ𕶎���Ŏ擾 @endja
    /*!@ja 
    "YYYY/MM/DD hh:mm:ss.msec"�`���̕�������擾���܂��B���O�o�͂Ɏg�p���܂��B
    @return ���Ԃ̕�����
    @endja
    */
	vce::g_string NowTimeString();

	typedef std::vector<std::string> StringList;
    //!@ja �����񕪊� @endja
    /*!@ja 
	delim�ŕ�����𕪊����܂��B�������Aescape�ň͂܂ꂽ�����͂Ђƌł܂�̕�����Ƃ��Ĉ����Adelim���܂�ł��Ă���������܂���B
	escape�͏o�͕�����Ɋ܂܂�܂���B
    �����l�����̂܂܂Ȃ�΁A���Ƃ��� 'aaa bbb "c c c"' �́A'aaa' 'bbb' 'c c c' ��3�ɕ�������܂��B
    @param string ���͕�����B
    @param delim ��؂蕶���B�����l��' '�B
    @param escape �G�X�P�[�v�����B�����l��'"'�B
    @return �������ꂽ������
    @endja
    */
    StringList SplitString(const std::string& string, char delim = ' ', char escape = '"');
	typedef std::map<std::string,std::string> PostMap;
    //!@ja URL�̃N�G���[������`������͂��� @endja
    /*!@ja 
    HTTP POST���N�G�X�g�ȂǂɎg����N�G���[������`������͂��āA�L�[�ƒl�̃y�A�ɕ������܂��B�Ⴆ�� 'aaa=bb&ccc=a%20bc' ��2�ɕ�������܂��B'%XX' ��URI�f�R�[�h����܂��B
    @endja
    */
    PostMap SplitPost(const std::string& _post);

    //!@ja URI�G���R�[�h @endja
    /*!@ja 
    a-z,A-Z,0-9���������ׂĂ�URI�G���R�[�h���܂��B
	@return �ϊ����ꂽ������
    @endja
    */
    std::string encodeURI(const std::string& text);
    //!@ja URI���f�R�[�h @endja
    /*!@ja 
	URI�G���R�[�h���ꂽ��������f�R�[�h���܂��B
	@return �ϊ����ꂽ������B���s����Ƌ�
    @endja
    */
    std::string decodeURI(const std::string& text);

	//!@ja Base64�G���R�[�h @endja
	/*!@ja 
	�o�C�i���f�[�^��Base64�G���R�[�h���܂��B�߂�l��Null�I�[���ꂽ������ŁA���f�[�^��4/3�{���x�ɂȂ�܂��B
	@param data �o�C�i���f�[�^�ւ̃A�h���X
	@param len �o�C�i���f�[�^�̒���
	@return �ϊ�����������
	@endja
	*/
	std::string Base64Encode(const void *data,int len);

	//!@ja Base64Decode @endja
	/*!@ja 
	Base64�G���R�[�h���ꂽ��������f�R�[�h���܂��B
	@param src Base64�G���R�[�h���ꂽ������B
	@param dest �i�[����m�ۍςݗ̈�ւ̃|�C���^�B�Œ�A������̒�����(len/4+1)*3�o�C�g�m�ۂ��Ă����܂��B
	@return �ϊ����������A���s�����ꍇ-1�B
	@endja
	*/
	int Base64Decode(const char *src,unsigned char *dest);
	std::string Base64DecodeString(const std::string &src);
	int Base64Decode(const char *src,std::vector<unsigned char> & dest);

	//!@ja �o�C�i���f�[�^��16�i������ϊ� @endja
	/*!@ja 
	�ϊ����ꂽ�������2�����P�ʂŁA�A���t�@�x�b�g��16�i���͑啶���ŏo�͂���܂��B
	@param data �ϊ�����f�[�^�ւ̐擪�|�C���^
	@param len �ϊ�����o�C�g��
	@return �ϊ����ꂽ������
	@endja
	*/
	std::string MemoryToStr(const void *data,size_t len);

	class Random//final
	{
		void *context;
	public:

		friend class BlockEncryptCamellia;
		friend class BlockEncryptBlowfish;
		friend class BlockEncryptRijndael;
		friend class RSAEncrypt;

		/*!@ja
		�R���X�g���N�^�B���ԌW���ŋ[�������̎��ݒ肵���������܂��B
		@endja
		*/
		Random();
		/*!@ja
		�R���X�g���N�^�Bseed�����ɋ[�������̎��ݒ肵�܂��B
		@param seed ��̒l���������o�b�t�@�ւ̃|�C���^�B�����킩��͓����p�^�[���ŋ[����������������܂��B
		@param seedsize ��̃o�b�t�@�̃o�C�g��
		@endja
		*/
		Random(unsigned char *seed,size_t seedsize);
		/*!@ja
		�f�X�g���N�^
		@endja
		*/
		~Random();
		/*!@ja
		�[�������𐶐����܂��B
		@return �������ꂽ�l�B0~2^32�͈̔͂ɂȂ�܂��B
		@endja
		*/
		unsigned int rand();
	};

	class RSAEncrypt
	{
		void *context;
		Random* random;
		RSAEncrypt(const RSAEncrypt&);
		RSAEncrypt& operator=(RSAEncrypt&);
	public:
		RSAEncrypt();
		/* FIXME: add description */
		explicit RSAEncrypt(Random* rand);
		~RSAEncrypt();
		/*!@ja
		���𐶐����܂��B���̃r�b�g����128�̔{������128�ȏ�A1024�ȉ��ł���K�v������܂��B
		@param generatebits ���̃r�b�g��
		@return ���������ꍇ��true�B
		@endja
		*/
		bool GenerateKey(int generatebits);
		/*!@ja
		�������ꂽ����ݒ肵�܂��B
		@param key ���̃o�b�t�@
		@param keybits ���̃r�b�g��
		@return ���������ꍇ��true�B
		@endja
		*/
		bool SetPublicKey(const unsigned char *key,int keybits);
		/*!@ja
		�������ꂽ�����擾���܂��B
		@param key �����󂯎��o�b�t�@�B�o�b�t�@�͍Œ�ł����̃r�b�g��/8�̃o�C�g���m�ۂ���Ă���K�v������܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		bool GetPublicKey(unsigned char *key)const;
		/*!@ja
		�Í������܂��B�Ō��1�o�C�g�̓[���ɂ��Ă����܂��B������key�ƃf�[�^�̑g�ݍ��킹�ɂ���Ă�
		�Í����Ɏ��s���邱�Ƃ�����܂��B
		@param buf �Í�������f�[�^���������o�b�t�@�B�o�b�t�@�͍Œ�ł����̃r�b�g��/8�o�C�g���m�ۂ���Ă���K�v������܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		bool Encrypt(unsigned char *buf);
		/*!@ja
		�Í������ꂽ�f�[�^�𕜍������܂��B
		@param buf ����������f�[�^���������o�b�t�@�B
		@return ���������ꍇ��true�B
		@endja
		*/
		bool Decrypt(unsigned char *buf);
	};


    /*!@ja
    �Í������������߂̒��ۃN���X�B���̃N���X���g���đ��삵�܂��B 
    �I�u�W�F�N�g�𐶐�����Ƃ��͔h���N���X�Ő�������K�v������܂��B 
    @endja 
    */
 	class BlockEncrypt
	{
	protected:
		BlockEncrypt();
	public:
		virtual ~BlockEncrypt();
        /*!@ja 
        ���ƌ��̃r�b�g����ݒ肵�ď��������܂��BCamellia�ł�128��256�r�b�g�ABlowfish�ł�96����448�r�b�g�܂Ŏw��ł��܂��B 
        @param key ���ւ̃o�b�t�@ 
        @param keybitlength ���̃r�b�g�� 
        @return ���������ꍇ��true�B 
        @endja 
        */
        virtual bool Initialize(unsigned char *key,int keybitlength)=0;
		/*!@ja
		�Í����̓x��context�̓��e���ω����邽�߁A�������ł����Ă��Í����ʂ͕ς���Ă��܂��܂��B
		�������ŕ����̃f�[�^���Í����������Ƃ���Reinitialize�ŏ��������Ȃ����K�v������܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool Reinitialize()=0;
		/*!@ja
		�Í������܂��B
		@param src �Í�������f�[�^���������o�b�t�@�B
		@param srcsize �o�b�t�@�̃o�C�g���B
		@param dest �Í��������f�[�^������o�b�t�@�Bvector�ϐ��ւ̎Q�Ƃ�n���܂��B�����I��resize����܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool Encrypt(const void *src,size_t srcsize,std::vector<unsigned char> &dest)=0;
		/*!@ja
		���������܂��B
		@param src �Í������ꂽ�f�[�^��������vector�B
		@param dest ���������ꂽ�f�[�^������o�b�t�@�Bvector�ϐ��ւ̎Q�Ƃ�n���܂��B�����I��resize����܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool Decrypt(const std::vector<unsigned char> &src,std::vector<unsigned char> &dest)=0;
		/*!@ja
		�Í�������f�[�^��vector�ɂ��A�l�X�Ȍ^���w��ł���悤�ɂ������̂ł��B
		@sa Encrypt
		@endja
		*/
		template<typename Type>bool Encrypt(const std::vector<Type> &src,std::vector<unsigned char> &dest)
		{
			return Encrypt(&*src.begin(),src.size()*sizeof(Type),dest);
		}
	};


	class BlockEncryptCamellia
		:public BlockEncrypt
	{
		void *context;
		Random* random;
	public:
		/*!@ja
		Camellia�A���S���Y�����g�����߂̃I�u�W�F�N�g�𐶐����܂��B�����ɂ�128�r�b�g��256�r�b�g���w��ł��܂��B
		���̃R���X�g���N�^�ō\�z�����I�u�W�F�N�g�̓X���b�h�Z�[�t�ł͂���܂���B
		@endja
		*/
		BlockEncryptCamellia();
		/*!@ja
		 �����I�u�W�F�N�g���w�肷��R���X�g���N�^�ł��B
		 ���̃R���X�g���N�^�ō\�z�����I�u�W�F�N�g�̓X���b�h�Z�[�t�ł��B
		@endja
		*/
		explicit BlockEncryptCamellia(Random* rand);
		~BlockEncryptCamellia();

		bool Initialize(unsigned char *key,int keybitlength);
		bool Reinitialize();
		bool Encrypt(const void *src,size_t srcsize,std::vector<unsigned char> &dest);
		bool Decrypt(const std::vector<unsigned char> &src,std::vector<unsigned char> &dest);
	};

	class BlockEncryptBlowfish
		:public BlockEncrypt
	{
		void *context;
		Random* random;
	public:
		/*!@ja
		Blowfish�A���S���Y�����g�����߂̃I�u�W�F�N�g�𐶐����܂��B�����ɂ�96����448�r�b�g�܂Ŏw��ł��܂��B
		���̃R���X�g���N�^�ō\�z����
		�I�u�W�F�N�g�̓X���b�h�Z�[�t�ł͂���܂���B
		@endja
		*/
		BlockEncryptBlowfish();
		/*!@ja
		�����I�u�W�F�N�g���w�肷��R���X�g���N�^�ł��B
		���̃R���X�g���N�^�ō\�z�����I�u�W�F�N�g�̓X���b�h�Z�[�t�ł��B
		@endja
		*/
		explicit BlockEncryptBlowfish(Random* rand);
		~BlockEncryptBlowfish();

		bool Initialize(unsigned char *key,int keybitlength);
		bool Reinitialize();
		bool Encrypt(const void *src,size_t srcsize,std::vector<unsigned char> &dest);
		bool Decrypt(const std::vector<unsigned char> &src,std::vector<unsigned char> &dest);
	};

	class BlockEncryptRijndael
		:public BlockEncrypt
	{
		void *context;
		Random* random;
	public:
		/*!@ja
		Rijndael�A���S���Y�����g�����߂̃I�u�W�F�N�g�𐶐����܂��B128�A192�A256�r�b�g�̌������w��ł��܂��B
		���̃R���X�g���N�^�ō\�z�����I�u�W�F�N�g�̓X���b�h�Z�[�t�ł͂���܂���B
		@endja
		*/
		BlockEncryptRijndael();
		/*!@ja
		�����I�u�W�F�N�g���w�肷��R���X�g���N�^�ł��B
		���̃R���X�g���N�^�ō\�z�����I�u�W�F�N�g�̓X���b�h�Z�[�t�ł��B
		@endja
		*/
		explicit BlockEncryptRijndael(Random* rand);
		~BlockEncryptRijndael();

		bool Initialize(unsigned char *key,int keybitlength);
		bool Reinitialize();
		bool Encrypt(const void *src,size_t srcsize,std::vector<unsigned char> &dest);
		bool Decrypt(const std::vector<unsigned char> &src,std::vector<unsigned char> &dest);
	};

	//!@ja ���k���s���C���^�[�t�F�C�X @endja
	class CompressBase
	{
	protected:
		//!@ja context @endja
		void *context;
	public:
		//!@ja ���ڐ��������A�h���N���X�𐶐�����悤�ɂ��Ă��������B @endja
		CompressBase();
		//!@ja �f�X�g���N�^ @endja
		virtual ~CompressBase();
		//!@ja ���k @endja
		/*!@ja
		�o�C�i���f�[�^�����k���܂��B
		���f�[�^�����G�ȃf�[�^�ł�������A����������f�[�^�ł������肷��ƈ��k�ł����Ɏ��s���܂��B
		���k�Ɏ��s�����Ƃ��͏o�̓f�[�^���������Ȃ��̂ŁAdest��Uncompress�ɓn���Ă͂����܂���B
		@param src ���k�Ώۂ̃o�b�t�@
		@param srcsize �o�b�t�@�̃o�C�g��
		@param dest ���k���ꂽ�f�[�^���󂯎��o�b�t�@�Avector�z��ւ̎Q�Ƃ�n���܂��B
		@return ���������ꍇ��true�B���k�Ɏ��s�����ꍇ��false�B���f�[�^���o�̓f�[�^���傫���Ȃ����ꍇ��false���Ԃ�܂��B
		@endja
		*/
		virtual bool Compress(const void *src,size_t srcsize,std::vector<unsigned char> &dest)=0;
		//!@ja ���k�֐��̃e���v���[�g�� @endja
		/*!@ja
		@param src ���k�Ώۂ̃o�b�t�@
		@sa Compress
		@endja
		*/
		template <typename T>bool Compress(const std::vector<T> &src,std::vector<unsigned char> &dest)
		{
			return src.size()>0?Compress((void*)&src.front(),sizeof(T)*src.size(),dest):false;
		}
		//!@ja �W�J @endja
		/*!@ja
		@param src Compress�ɂ�舳�k���ꂽ�o�b�t�@
		@param dest �W�J�����f�[�^���󂯎��o�b�t�@�Bvector�z��ւ̎Q�Ƃ�n���܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool Uncompress(const std::vector<unsigned char> &src,std::vector<unsigned char> &dest)=0;
		//!@ja �W�J�֐��̃e���v���[�g�� @endja
		/*!@ja
		���܂��܂Ȍ^��vector�ɑΉ��ł��܂��B������Compress�Ŏg���^�ƍ��킹��K�v������܂��B
		@endja
		*/
		template <typename T>bool Uncompress(const std::vector<unsigned char> &src,std::vector<T> &dest)
		{
			std::vector<unsigned char> temp;
			if(Uncompress(src,temp))
			{
				dest.resize(temp.size()/sizeof(T));
				std::memcpy(&dest.front(),&temp.front(),temp.size());
				return true;
			}
			return false;
		}

	private:
		CompressBase(const CompressBase&);
		CompressBase& operator=(CompressBase&);
	};

	class CompressLZP
		:public CompressBase
	{
	public:
		CompressLZP();
		~CompressLZP();

		bool Compress(const void *src,size_t srcsize,std::vector<unsigned char> &dest);
		bool Uncompress(const std::vector<unsigned char> &src,std::vector<unsigned char> &dest);
	};

	class CompressZIP
		:public CompressBase
	{
	public:
		CompressZIP();
		~CompressZIP();

		bool Compress(const void *src,size_t srcsize,std::vector<unsigned char> &dest);
		bool Uncompress(const std::vector<unsigned char> &src,std::vector<unsigned char> &dest);
	};



    //!@ja VCE���������N���X @endja
    /*!@ja 
    �����ŊǗ������邽�߂̃x�[�X�N���X�B
    ���[�U�����̃N���X�𒼐ڎg�p���邱�Ƃ͂���܂���B
    @endja
    */

    class Base
    {
    protected:
		//!@ja �R���X�g���N�^ @endja
        Base();
		//!@ja �f�X�g���N�^ @endja
        virtual ~Base();
	private:
		Base(const Base&);
		Base& operator=(Base&);
    public:
		//!@ja �����I�u�W�F�N�g�ւ̃|�C���^�̃L���b�V�� @endja
        void*cache;
        //!@ja �ŗL���ʎq @endja
        VUint32 uID;
        //!@ja ���̃I�u�W�F�N�g���Ǘ����Ă���VCE�I�u�W�F�N�g @endja
        VCE *api;

		//!@ja VCE �����Ǘ�����n�߂���Ă΂��R�[���o�b�N�֐� @endja
		/*!@ja 
        Connect �� Listen ���ɓn�����Ƃ��ɌĂ΂�܂��B
		���ꂪ�Ă΂�Ă��� Detached ���Ă΂��܂� delete ���Ă͂����܂���B
		@sa Detached
		@endja
		*/
        virtual void Attached();
        //!@ja VCE �����Ǘ�����؂藣���ꂽ�Ƃ��ɌĂ΂��R�[���o�b�N�֐� @endja
        /*!@ja 
        Connect �� Listen ���̈����ɓn�����I�u�W�F�N�g�� VCE �����Ŏg�p����Ȃ��Ȃ�ƌĂ΂�܂��B
        Detached ���Ă΂ꂽ��ł���� delete ���邱�Ƃ��ł��܂��B
		@sa Attached
        @endja
        */
        virtual void Detached();
        //!@ja �v�l�֐� @endja
        /*!@ja 
        vce::Poll���Ă΂ꂽ�Ƃ��ɂ��̊֐����Ă΂�܂��B
		���[�U���D���Ȃ悤�ɃI�[�o�[���C�h���邱�Ƃ��ł��܂��B
        @endja
        */
        virtual void Think();

		//!@ja �����I�u�W�F�N�g����r���܂��B @endja
		/*!@ja
		VCE�Ŏg�p�����I�u�W�F�N�g�͌ŗL��ID��ێ����Ă���A���̔�r�����܂��B
		���̔�r���ʂ��^�ł���Γ����Z�b�V�����������̓��X�i�[�Ƃ������ƂɂȂ�܂��B
		�|�C���^�����̔�r�ł͕���ꂽ�Z�b�V�����Ɠ����A�h���X�ɍĊm�ۂ���邱�Ƃ�����̂�
		VCE�ň�ӂɊǗ�����Ă���ID�Ŕ�r����K�v������܂��B
		@endja
		*/
        bool operator ==(Base &target)const;
		//!@ja �Ⴄ�I�u�W�F�N�g����r���܂��B @endja
        bool operator !=(Base &target)const;
        //!@ja �����I�u�W�F�N�g����r���܂��B @endja
        bool IsEqual(Base &target)const;

        //!@ja ��Ԃ��擾 @endja
        /*!@ja 
		��Ԃ��擾���܂��B�Z�b�V�����̏�Ԃ�PREPROCESS��CONNECTING��ESTABLISHED��CLOSED�̂悤�ɑJ�ڂ��Ă����܂��B
		���X�i�[�ł�PREPROCESS��LISTENING��CLOSED�̂悤�ɑJ�ڂ��Ă����܂��B
		PREPROCESS�̊Ԃɖ��O������|�[�g�ւ̃o�C���h�������Ȃ��܂��B
		CLOSED�ɂȂ�Ƃ���VCE����I�u�W�F�N�g���폜����Ă��܂����߁A���ۂɂ��̊֐���CLOSED���Ԃ��Ă���^�C�~���O�́AClosed�֐����R�[���o�b�N���ꂽ�Ƃ������ł��B
		@sa VCE_STATE
        @endja
        */
        virtual VCE_STATE GetState()const;

        //!@ja ��O�擾�p�֐� @endja
        /*!@ja 
        �N���e�B�J���łȂ���O���N�������Ƃ��ɌĂ΂�܂��B
		@sa VCE_EXCEPTION
        @endja
        */
        virtual void Exception(VCE_EXCEPTION type);

        //!@ja socket�n���h�����擾���܂��B @endja
        int GetSocket();

        //!@ja VCE�����Ŏg���Ă��邩�`�F�b�N @endja
		/*!@ja
		true�̏ꍇ��VCE�����Ŏg���Ă���̂�delete���Ă͂����܂���B
		@deprecated ���̊֐��͔p�~�\��ł��B������ VCE::IsUsing(Base*) ���g�p���Ă��������B
		@endja
		*/
        bool IsUsing();

        //!@ja Poll ���̏����D��x��ύX���܂� @endja
		/*!@ja
		Poll ���̏����D��x��ύX���܂��B Attached ��ɐݒ��ς��邱�Ƃɂ�� Poll ����Ƃ��ɗD�悵�ď�������܂��B
		�ꎞ�I�ɑ�ʂ̃f�[�^�̑���M����������Ƃ��ɁA�ꕔ�̃Z�b�V�����������߂ɏ��������邱�Ƃ��ł��܂��B
		@param priority 1�ɂ��邱�ƂŗD�悵�ď������܂��B�f�t�H���g�� 0 �ł��B
		@endja
		*/
		void SetPollingPriority(int priority);
    };


    //!@ja VCE���X�i�[�N���X @endja
    /*!@ja 
    Listening���̃|�[�g�ւ̐ڑ��v���ɑ΂��铮����`���邽�߂̃N���X�B
	@sa vce::Listen
    @sa AutoAccepter
    @endja
    */
    class Listener
        :public Base
    {
    public:
		//!@ja �R���X�g���N�^ @endja
        Listener();
		//!@ja �f�X�g���N�^ @endja
        virtual ~Listener();
        //!@ja �ڑ��v�� @endja
        /*!@ja 
        ���̊֐����I�[�o�[���C�h���A�ڑ����󂯂�Z�b�V�����N���X��Ԃ��܂��B�ڑ������ۂ���ꍇ��NULL��Ԃ��܂��B
        @param remote_addr �ڑ����Ă����A�h���X
        @param remote_port �ڑ����Ă����|�[�g
        @return NULL�Ȃ狑��
        @endja
        */
        virtual Session *Accepter(VUint32 remote_addr,VUint16 remote_port)=0;

		//!@ja Close�̌�Ă΂��R�[���o�b�N�֐� @endja
		/*!@ja
		�ڑ����󂯓������|�[�g����A�ȍ~�󂯓���Ȃ��悤�ɂ��܂��B
		UDP���X�i�[�̏ꍇ�͊��Ɏ󂯓���ς݂̃Z�b�V�������ʐM�ł��Ȃ��Ȃ�A
		�܂��A�Z�b�V������parent�I�u�W�F�N�g���폜����Ă��܂��̂Œ��ӂ��K�v�ł��B
		@return �Ȃ�
		@endja
		*/
		virtual void Closed();

		//!@ja �q�Z�b�V������ Detached ���Ă΂��O�ɌĂ΂��R�[���o�b�N�֐� @endja
		virtual void ChildDetached(VUint32 ChildID);

		//!@ja ���X�j���O�𒆒f���� @endja
		/*!@ja 
		���X�j���O����߂ă|�[�g����܂��B
		���łɐڑ����ꂽ�Z�b�V�����͎c��܂��B
		@endja
		*/
        bool Close();
        //!@ja �Z�b�V�������A���X�g�擾 @endja
        /*!@ja 
        ���̃��X�i�[����h�������Z�b�V�����ꗗ���擾���܂��B
        @param child �󂯎��|�C���^�z��
        @param array_maxlen �󂯎��z��ɓ������ő�v�f��
        @return �Z�b�V�����̐�,�Ⴕ���͔z��ɃR�s�[������
        @endja
        */
        unsigned int GetChildren(Session **child,unsigned int array_maxlen);
		//!@ja �Z�b�V�������擾 @endja
		/*!@ja 
        @return �Z�b�V������
		@endja
		*/
        unsigned int GetChildren();
		//!@ja ��Ԏ擾 @endja
		/*!@ja 
        @return ���
		@endja
		*/
        VCE_STATE GetState()const;
		//!@ja GetChildren��vector�� @endja
		/*!@ja 
		Session�^��vector�������ɓn���Ƃ����փZ�b�V�����ꗗ���l�߂܂��B�����I��resize����܂��B
		@param ch �z��
		@endja
		*/
        template <typename T> void GetChildren(std::vector<T*> &ch)
        {
            ch.resize(GetChildren());
			if(ch.size())GetChildren((Session**)&ch[0],(unsigned int)ch.size());
        }

		typedef std::map<std::string,std::string> ProxyConfigParameter;
		//!@ja Proxy�̐ݒ��ς��� @endja
		/*!@ja
		@param function �ς������ݒ�̖��O
		@param parameter �p�����[�^�̏W��
		@return ���M�Ɏ��s�����Ƃ���false
		@endja
		*/
		bool ProxyConfig(const std::string& function,ProxyConfigParameter parameter);

        //!@ja ���X�i�[��IP�A�h���X���擾 @endja
        /*!@ja 
        @param addr IP�A�h���X���i�[����ϐ��ւ̎Q��
        @param port �|�[�g�ԍ����i�[����ϐ��ւ̎Q��
        @return �擾�ł�����true�A�ł��Ȃ�������false�B
        @endja
        */
        bool GetListenAddress(VUint32 &addr,VUint16 &port);
	
	};

	//!@ja UDP��Ԏ擾�p�\���� @endja
	struct UdpStatus
	{
		//!@ja ���M���悤�Ƃ����w�b�_���݂̃o�C�g�� @endja
		VUint32 sendsize;
		//!@ja ���M���悤�Ƃ����� @endja
		VUint32 sendcount;
		//!@ja �đ��M���悤�Ƃ����o�C�g�� @endja
		VUint32 resendsize;
		//!@ja �đ��M���悤�Ƃ����� @endja
		VUint32 resendcount;
		//!@ja ��M�����w�b�_���݂̃o�C�g�� @endja
		VUint32 recvsize;
		//!@ja ��M�����S�p�P�b�g�̌� @endja
		VUint32 recvcount;
		//!@ja ��M�����s�v�p�P�b�g�̌� @endja
		VUint32 dropcount;
	};

    //!@ja VCE�X�g���[���Z�b�V�����N���X @endja
    /*!@ja 
    �Z�b�V�����𑀍삷��̂Ɏg���܂��B���̃N���X���p�����A�e���z�֐����I�[�o�[���C�h���Ă����܂��B
	@sa VCE::Connect
    @sa Listener::Accepter
    @endja
    */

    class Session
        :public Base
    {
    public:
		//!@ja �Í������������ @endja
		enum KEYEX_STATE
		{
			//!@ja �s�� @endja
			KEYEX_UNKNOWN,
			//!@ja �ڑ��������Ă��� @endja
			KEYEX_CONNECTED,
			//!@ja �L�[��������,�L�[���M�� @endja
			KEYEX_KEYGENERATED,
			//!@ja key���M���� @endja
			KEYEX_KEYSENDED,
			//!@ja RSA�L�[�҂� @endja
			KEYEX_WAITING,
			//!@ja RSA�L�[���M�� @endja
			KEYEX_KEYSENDING,
			//!@ja �������I�� @endja
			KEYEX_READY
		};
		//!@ja �R���X�g���N�^�Őݒ肷��Z�b�V�����̃t���O�B @endja
        enum LowLevelFlags
        {
			//!@ja �����Ȃ� @endja
            LLF_NONE=0,
			//!@ja Camellia�Í��� @endja
			LLF_ENCRYPT_CAMELLIA=0x01,
			//!@ja Blowfish�Í��� @endja
			LLF_ENCRYPT_BLOWFISH_128BIT=0x02,
			//!@ja Rijndael�Í��� @endja
			LLF_ENCRYPT_RIJNDAEL=0x03,
			//!@ja VCE2�f�t�H���g�̈Í��� @endja
            LLF_ENCRYPT_DEFAULT=LLF_ENCRYPT_CAMELLIA,
			//!@ja LZP���k @endja
			LLF_COMPRESS_LZP=0x10,
			//!@ja ZIP���k @endja
			LLF_COMPRESS_ZIP=0x20
        };
		unsigned int llflags;//!<@ja �t���O @endja

	private:
		//!@ja �ڑ���� @endja
		enum ConnectingStatus
		{
			CS_WAIT,
			CS_SUCCESS,
			CS_FAIL
		}connectingstatus;//!<@ja �ڑ���� @endja
	public:

        //!@ja ���X�i�[�ւ̃|�C���^ @endja
        /*!@ja 
        �p�b�V�u�ڑ��̏ꍇ�̓��X�j���O���Ă����e�N���X�ւ̃|�C���^������܂��B
        �A�N�e�B�u�ڑ��̏ꍇ��NULL������܂��B
        @endja
        */
        Listener *parent;

		//!@ja �R���X�g���N�^ @endja
        Session();
		//!@ja �f�X�g���N�^ @endja
		virtual ~Session();

		void Exception(vce::VCE_EXCEPTION type);

        //!@ja �Z�b�V�����������I�ɕ��� @endja
        /*!@ja 
        �Z�b�V�������������܂��B
        @sa Close
        @sa Closed
        @endja
        */
        void ForceClose();
        //!@ja �Z�b�V��������� @endja
        /*!@ja 
        �����ؒf�����A���M�o�b�t�@����ɂȂ��Ă��瑗�M����A��M���I����Ă����M����܂��B
        �P���̃��N�G�X�g���o���A����ɑ΂��郌�X�|���X�������󂯎�鎞���Ɏg�p���܂��B
		@sa ForceClose
        @sa Closed
        @endja
        */
        void Close();
        //!@ja �ڑ����s���~ @endja
        /*!@ja 
        Connect������Atimeout��҂����ɂ�����߂�Ƃ��Ɏg���܂��B
		���̌��Exception���R�[�������VCE_EXCEPT_CONNECTFAIL���n����܂��B
		@sa VCE::Connect
        @endja
        */
        void CancelConnecting();
		//!@ja �u���b�L���O���� @endja
		/*!@ja
		�ڑ������������͎��s����܂Ńu���b�L���O���܂��B�p���悩��x�[�X�N���X��Exception��Connected���Ăяo���̂�Y���Ɛ���ɓ��삵�Ȃ����Ƃ�����܂��B
		@return true�Ȃ�ڑ�����
		@sa VCE::Connect
		@endja
		*/
		bool BlockConnecting();

        //!@ja �f�[�^���M @endja
        /*!@ja 
        �f�[�^�𑗐M�o�b�t�@�ɏ������݁A���M���܂��B
        @param p ���M�f�[�^�ւ̐擪�|�C���^�B
        @param sz ���M�f�[�^�̃o�C�g���B
        @return ���������ꍇ��true�A���s�����ꍇ��false�B
        @endja
        */
        bool Send(const void *p,size_t sz);
        //!@ja ��M�o�b�t�@�̃f�[�^���Q�� @endja
        /*!@ja 
        ��M�o�b�t�@�̓��e�𒼐ڎQ�Ƃ���ɂ͂��̊֐����g���܂��B
        @param size ��M�o�b�t�@�̃o�C�g���B
        @return ��M�o�b�t�@�ւ̐擪�|�C���^�ANULL�̏ꍇ�͎��s�B
		@sa EraseReadbuf
        @endja
        */
        const VUint8 *GetReadbuf(size_t &size);
        //!@ja ��M�o�b�t�@�̃f�[�^���폜 @endja
        /*!@ja 
        ��M�o�b�t�@�̐擪����f�[�^���폜���鎖���ł��܂��B
        @param size �폜����o�C�g��
		@sa GetReadbuf
        @endja
        */
        void EraseReadbuf(size_t size);

        //!@ja �ڑ������������Ƃ��ɌĂ΂��R�[���o�b�N�֐� @endja
        virtual void Connected();
        //!@ja �ڑ��������Ƃ��ɌĂ΂��R�[���o�b�N�֐� @endja
        virtual void Closed(VCE_CLOSEREASON type);
        //!@ja �f�[�^��M�R�[���o�b�N�֐� @endja
        /*!@ja 
        ���̉��z�֐����I�[�o�[���C�h����M�����������܂��B
        @param p ��M�f�[�^�ւ̃|�C���^
        @param sz ��M�f�[�^�̃o�C�g��
        @return ���������o�C�g���B0�Ȃ��M�o�b�t�@�̃f�[�^�͍폜����܂���B1�ȏ�Ȃ�o�b�t�@�̐擪����o�C�g�P�ʂō폜����܂��B
        @endja
        */
        virtual size_t Recv(const VUint8 *p,size_t sz);

        //!@ja �ڑ����IP�A�h���X���擾 @endja
        /*!@ja 
        @param addr IP�A�h���X���i�[����ϐ��ւ̎Q��
        @param port �|�[�g�ԍ����i�[����ϐ��ւ̎Q��
        @return �擾�ł�����true�A�ł��Ȃ�������false�B
        @endja
        */
        bool GetRemoteAddress(VUint32 &addr,VUint16 &port);

		//!@ja �ڑ���̃z�X�g�����擾 @endja
		/*!@ja
		�ڑ����Ɏg�p�����z�X�g�����擾�ł��܂��B
		�t�����ɂ͑Ή����Ă��Ȃ��̂ŋt�������K�v�ȏꍇ�͎擾�ł��܂���B
		@return �z�X�g���A�����ꍇ�͋󕶎���B
		@endja
		*/
		std::string GetRemoteHostname()const;

		//!@ja Nodelay��L���ɂ��܂� @endja
		/*!@ja
		TCP�X�g���[���łׂ͍����f�[�^��Z���Ԃő��M����ƃp�P�b�g�̃w�b�_���̃I�[�o�[�w�b�h���ƂĂ��傫���Ȃ�܂��B
		���̂���Nodelay���W���Ŗ����ɂȂ��Ă���ANagle�A���S���Y���ɏ]���čׂ����f�[�^����U�o�b�t�@��
		��莞�ԉ߂�����܂Ƃ߂Č����悭���M����悤�ɂȂ��Ă��܂��B�������o�b�t�@���ꂽ�f�[�^�͈�莞�ԉ߂��Ȃ��Ƒ��M����Ȃ����߁A
		�{���ɒZ���ԂŃf�[�^�𑗐M�������ꍇ��Nodelay��L���ɂ���悤�ɂ��܂��B
		���̊֐���Connected�R�[���o�b�N��Ɏg�����Ƃ��ł��܂��B���������ɂ���Ă͐ݒ��ς����Ȃ��\��������܂��B
		@param enable Nodelay��L���ɂ���Ƃ���true
		@return ���������ꍇ��true�B
		@endja
		*/
		bool SetNodelay(bool enable=true);

        //!@ja �ڑ�����IP�A�h���X���擾 @endja
        /*!@ja 
        @param addr IP�A�h���X���i�[����ϐ��ւ̎Q��
        @param port �|�[�g�ԍ����i�[����ϐ��ւ̎Q��
        @return ���s��false
        @endja
        */
        bool GetLocalAddress(VUint32 &addr,VUint16 &port)const;

        //!@ja ���M�o�b�t�@�̋󂫗e�� @endja
        /*!@ja 
        ���M�o�b�t�@�̎g�p�ʂ�������ƃ������Ċm�ۂ��������邽�߁A�V�r�A�ȏ󋵂ł͂��̊֐��Ń`�F�b�N���Ȃ��瑗�M����悤�ɂ��܂��B
        @return ���M�o�b�t�@�󂫗e��
        @endja
        */
        size_t GetAvailableSendBuffer()const;

        //!@ja ���M�L���[�̍ő�l�̐ݒ� @endja
        /*!@ja 
        ��x�ɑ��M����ʂ�ݒ肵�܂��B���M�L���[�ɂ܂��f�[�^���c���Ă���Ƃ��͂���𒴂��Ȃ��悤�ɂ��܂��B
		1 ��� Poll �ł�葽�����M�������Ƃ���A����̃Z�b�V�����̑��M�ʂ��i�����肷��Ƃ��Ɏg���܂��B
		Connected ��ɐݒ肷�邱�ƂŗL���ɂȂ�܂��B���݁A Linux �̂ݑΉ����Ă��܂��B
		���̃v���b�g�t�H�[���ł͑��M�L���[�̗��p�ʂɂ�����炸���M�L���[�ɒǉ�����܂��B
		@param limit ���M�L���[�̃T�C�Y�B�f�t�H���g�� 16384 �ł��B
        @endja
        */
        bool SetSendQueueLimit(int limit);

		//!@ja ���M�o�b�t�@�T�C�Y�̐ݒ� @endja
		/*!@ja
		���M�o�b�t�@�T�C�Y��ݒ肵�܂��B�����ɉϒ����ǂ����A�ϒ��Ȃ�΍ő�̗e�ʂ�
		���o�C�g���̐ݒ�����܂��BConnected�R�[���o�b�N��ɐݒ肵�܂��B
		�R���X�g���N�^�ŌĂԂƐݒ�͔��f���ꂸ�A����M���n�܂��Ă���ł�
		�w�肵���e�ʂɐݒ�ł��Ȃ��ꍇ������܂��B
		@param size �o�b�t�@�T�C�Y
		@param variable �o�b�t�@�T�C�Y���ϒ��ɂ��邩�̎w��Btrue�ɂ���Ƒ傫�ȃf�[�^���������܂ꂽ�Ƃ��Ƀo�b�t�@������Ȃ����ɑ傫���Ȃ�A
		���ԂƂƂ��Ƀo�b�t�@���������Ȃ�܂��Bcapacity�𒴂���T�C�Y���m�ۂ���邱�Ƃ͂���܂���B
		@param capacity �o�b�t�@��傫������ꍇ�͉��o�C�g�܂ŋ��e���邩��ݒ肵�܂��B
		�o�b�t�@�T�C�Y���傫���K�v������܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		bool SetSendBufferSize(size_t size,bool variable=true,size_t capacity=1024*1024*16);
		//!@ja ��M�o�b�t�@�T�C�Y�̐ݒ� @endja
		/*!@ja
		��M�o�b�t�@�T�C�Y��ݒ肵�܂��B�����ɉϒ����ǂ����A�ϒ��Ȃ�΍ő�̗e�ʂ�
		���o�C�g���̐ݒ�����܂��BConnected�R�[���o�b�N��ɐݒ肵�܂��B
		�R���X�g���N�^�ŌĂԂƐݒ�͔��f���ꂸ�A����M���n�܂��Ă���ł�
		�w�肵���e�ʂɐݒ�ł��Ȃ��ꍇ������܂��B
		@param size �o�b�t�@�T�C�Y
		@param variable �o�b�t�@�T�C�Y���ϒ��ɂ��邩�̎w��Btrue�ɂ���Ƒ傫�ȃf�[�^���������܂ꂽ�Ƃ��Ƀo�b�t�@������Ȃ����ɑ傫���Ȃ�A
		���ԂƂƂ��Ƀo�b�t�@���������Ȃ�܂��Bcapacity�𒴂���T�C�Y���m�ۂ���邱�Ƃ͂���܂���B
		@param capacity �o�b�t�@��傫������ꍇ�͉��o�C�g�܂ŋ��e���邩�̐ݒ�B
		�o�b�t�@�T�C�Y���傫���K�v��������܂��B
		@return ���������ꍇ��true�B
		@endja
		*/
		bool SetRecvBufferSize(size_t size,bool variable=true,size_t capacity=1024*1024*16);

		//!@ja ���M�o�b�t�@�T�C�Y�̎擾 @endja
		/*!@ja
		���M�o�b�t�@�T�C�Y���擾���܂��B
		@return ���M�o�b�t�@�T�C�Y
		@endja
		*/
		virtual size_t GetSendBufferSize()const;
		//!@ja ���M�o�b�t�@�T�C�Y���ϒ����ǂ����̎擾 @endja
		/*!@ja
		���M�o�b�t�@�T�C�Y���ϒ����ǂ������擾���܂��B
		@return 0�Ȃ�Α��M�o�b�t�@�T�C�Y�͌Œ蒷�B
		@endja
		*/
		virtual int GetSendBufferVariable()const;
		//!@ja ���M�o�b�t�@�̍ő�e�ʂ̎擾 @endja
		/*!@ja
		���M�o�b�t�@�̍ő�e�ʂ��擾���܂��B
		@return ���M�o�b�t�@�̍ő�e��
		@endja
		*/
		virtual size_t GetSendBufferCapacity()const;
		//!@ja ��M�o�b�t�@�T�C�Y�̎擾 @endja
		/*!@ja
		��M�o�b�t�@�T�C�Y���擾���܂��B
		@return ��M�o�b�t�@�T�C�Y
		@endja
		*/
		virtual size_t GetRecvBufferSize()const;
		//!@ja ��M�o�b�t�@�T�C�Y���ϒ����ǂ����̎擾 @endja
		/*!@ja
		��M�o�b�t�@�T�C�Y���ϒ����ǂ������擾���܂��B
		@return 0�Ȃ�Ύ�M�o�b�t�@�T�C�Y�͌Œ蒷�B
		@endja
		*/
		virtual int GetRecvBufferVariable()const;
		//!@ja ��M�o�b�t�@�̍ő�e�ʂ̎擾 @endja
		/*!@ja
		��M�o�b�t�@�̍ő�e�ʂ��擾���܂��B
		@return ��M�o�b�t�@�̍ő�e��
		@endja
		*/
		virtual size_t GetRecvBufferCapacity()const;

		//!@ja UDP�̂݁B�^�C���A�E�g�ݒ� @endja
		/*!@ja
		�^�C���A�E�g���Ԃ�ݒ肵�܂��B���̎��ԓ��Ƀf�[�^����M����Ȃ����
		���葤�Z�b�V�����̕s��Ɣ��f���Đؒf���܂��B
		Connected�R�[���o�b�N��ɐݒ肷�邱�Ƃ��ł��܂��BTCP�̏ꍇ�̓A�v���P�[�V�������I�����ꂽ�Ƃ���OS���㏈��������̂�
		���̏ꍇ�͐�����Close����܂����AUDP�̓A�v���P�[�V�������I�������Ɛ�����Close�ł��Ȃ��̂�Timeout��҂��ƂɂȂ�܂��B
		@param time �~���b�P�ʂ̎��ԁB�����l��20000�~���b�B
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool UdpSetTimeoutTime(int time);
		//!@ja UDP�̂݁B�p�P�b�g�đ����Ԑݒ� @endja
		/*!@ja
		���肩��p�P�b�g��M�̊m�F�����������Ƃ��Ƀp�P�b�g���đ�����܂ł̎��Ԃ�ݒ肵�܂��B
		Connected�R�[���o�b�N��ɐݒ肷�邱�Ƃ��ł��܂��B
		@param time �~���b�P�ʂ̎��ԁB�����l��1000�~���b�B
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool UdpSetResendTime(int time);
		//!@ja UDP�̂݁B��������M�ʐݒ� @endja
		/*!@ja
		���肩�獇�}�Ȃ��ɑ��M����ʂ�ݒ肵�܂��B�傫���ݒ肷��ƈ�x�ɑ���M�ł���ʂ������Ȃ胍�X���₷���A���X�|���X���x�������܂��B
		�������ݒ肷��Ƒ傫���f�[�^�̂����ɂ͌����܂��񂪁A���X�������背�X�|���X�������Ȃ�܂��BTCP�̎�M���o�b�t�@�Ɠ����悤�ȓ��������܂��B
		Connected�R�[���o�b�N��ɐݒ肷�邱�Ƃ��ł��܂��B
		@param size �o�C�g�P�ʂ̃T�C�Y�B���ۂɂ̓p�P�b�g�̃T�C�Y�Ȃǂɍ��킹���肷��̂Ŏ኱������傫���Ȃ邱�Ƃ�����܂��B�����l��5720�ł��B
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool UdpSetTargetBufferSize(int size);
		//!@ja UDP�̂݁BUDP�ŗL�̏��擾�֐� @endja
		/*!@ja
		UDP�ŗL�̃Z�b�V���������擾���܂��B
		@param status UdpStatus�\���̂ւ̎Q�ƁB
		@return ����
		@sa UdpStatus
		@endja
		*/
		virtual bool UdpGetStatus(UdpStatus &status)const;

		//!@ja UDP �̂݁B�M�����̂Ȃ��f�[�^���M�ɐ؂�ւ��܂��B @endja
		/*!@ja
		�M�����̂Ȃ��f�[�^���M�ɐ؂�ւ��܂��B�Í��∳�k�Z�b�V�����̏ꍇ�͂��̊֐��𗘗p���Ă��M�����̂���f�[�^�Ƃ��đ��M����܂��B
		�Ȍ�A UdpFinishRaw ���Ă΂��܂ň�U�o�b�t�@�����O���܂��B 
		�ő�� 1400 �o�C�g�܂Ńo�b�t�@�����O����A 1400 �o�C�g�𒴂����Ƃ��͒����������j������܂��B
		���M�֐����ĂԑO�ɂ��̊֐����g���A���M�֐����Ă񂾌�� UdpFinishRaw ���g���Ă��������B
		@return ���������ꍇ�� true
		@sa UdpFinishRaw
		@endja
		*/
		bool UdpPrepareRaw();

		//!@ja UDP �̂݁B�M�����̂Ȃ��f�[�^���M�����܂��B @endja
		/*!@ja
		�o�b�t�@�����O���ꂽ�f�[�^�𑦍��ɑ��M���܂��B�K�� UdpPrepareRaw �Ƒ΂ɂ��Ďg���Ă��������B
		@return ���������ꍇ�� true
		@sa UdpPrepareRaw
		@endja
		*/
		bool UdpFinishRaw();

		//!@ja UDP �̂݁B�M�����̂Ȃ��f�[�^��M�����ׂ܂��B @endja
		/*!@ja
		gen �𗘗p����ƃt�H�[�}�b�g�ɍ���Ȃ��f�[�^����M���ꂽ�ꍇ�͐ؒf���Ă��܂��܂����A
		�M�����̂Ȃ��f�[�^�̏ꍇ�� RecvException �֐��̒��ł��̊֐����g���A�ؒf���邩�ǂ����̔��f�̐��x�����߂��܂��B
		@return �M�����̂Ȃ��f�[�^��M�̏ꍇ�� true ��Ԃ��܂��B
		@endja
		*/
		bool UdpRecvIsRaw();

        VCE_STATE GetState()const;

		//!@ja �������̏�Ԃ��擾���܂� @endja
		KEYEX_STATE GetKeyexState()const;
    };

    //!@ja �����Z�b�V�����J�n�N���X @endja
    /*!@ja 
    �ڑ��������Ƃ��Ɏ����ŃT�[�o�p�Z�b�V�����𐶐����A�Ԃ����߂̃e���v���[�g�N���X�B
    ���̃N���X���烊�X�i�[�𐶐����AListen�֐��Ɉ����Ƃ��ēn�������œ��삵�܂��B
	@sa net::Listen
    @endja
    */
    template <class T>class AutoAccepter:public Listener
    {
        bool enable;//!@ja �ڑ��������Ă��邩 @endja
        const unsigned int maxconnections;//!@ja �ő哯���ڑ��� @endja
    protected:
		//!@ja Accepter @endja
        virtual Session *Accepter(VUint32,VUint16){
            if(maxconnections&&GetChildren()>=maxconnections)
                return NULL;
            return enable?new T:NULL;};
    public:
		//!@ja �R���X�g���N�^ @endja
		/*!@ja 
		@param maxconn �ő哯���ڑ���
		@endja
		*/
        AutoAccepter(int maxconn=0):maxconnections(maxconn){
            enable=true;}
        //!@ja ����̐ڑ��������܂��B @endja
        void Enable(){
            enable=true;}
        //!@ja ����̐ڑ������ۂ��܂��B @endja
        void Disable(){
            enable=false;}
	private:
		AutoAccepter(const AutoAccepter&);
		AutoAccepter& operator=(AutoAccepter&);
		
    };

    //!@ja VCE�p�P�b�g�N���X @endja
    /*!@ja 
    Session�̔h���N���X�ŁA����M�X�g���[���f�[�^���p�[�X/�}�[�W���A�p�P�b�g�f�[�^�ɂ���@�\�������Ă��܂��B
    �p�P�b�g�P�ʂŃf�[�^�𑗐M(�}�[�W)�A����ю�M(�p�[�X)���ď������鎖���ł��܂��B
    �Ⴆ��5�o�C�g��2�񑗐M����ƁA�X�g���[���f�[�^�̂܂܂̏ꍇ�A��M���ł͋�؂薳����10�o�C�g����C�Ɏ�M����܂��B�p�P�b�g�f�[�^�Ȃ�΁A���M���Ɠ����傫���̃f�[�^����܂Ƃ܂�Ŏ�M���邱�Ƃ��ł��܂��B
	�ڑ���̃Z�b�V������Codec�N���X���p�����Ă���K�v������܂��B
    @endja
    */
    class Codec
        :public Session
    {
		//!@ja ping-pong���ԁB�~���b�P�ʂł��B @endja
		VUint64 ping;
		VUint64 pinginterval;
		VUint64 lastping;
		VUint64 timeout;
		VUint64 lastrecv;
	protected:
        //!@ja �����Œ�`����Ă����M�֐� @endja
        size_t Recv(const VUint8 *p,size_t sz);
    public:
        //!@ja �p�[�X���ꂽ�p�P�b�g����M���邽�߂̊֐� @endja
        /*!@ja 
        �p�P�b�g�P�ʂ̃f�[�^����M����ƌĂ΂�܂��B
        @param p ��M�f�[�^�ւ̃|�C���^
        @param sz
        @endja
        */
        virtual void Parsed(const VUint8 *p,size_t sz)=0;

		//!@ja �v�l�֐��B�p����ŕK���R�[������K�v������܂��B @endja
		void Think();

		//! initialize
		Codec();
		//! uninitialize
		~Codec();
		//!@ja ping-pong���Ԃ��擾���܂� @endja
		/*!@ja ping�𑗂�pong���Ԃ��Ă���܂ł̎��ԍ����~���b�P�ʂŕԂ��܂��B����I�ɍX�V����܂��B
		@return ping
		@endja
		*/
		VUint64 GetPingTime()const;
		//!@ja �^�C���A�E�g��ݒ肵�܂� @endja
		/*!@ja
		�^�C���A�E�g���ԓ��ɉ����f�[�^����M���Ȃ������Ƃ���Close���܂��Bping��interval��ݒ肵�Ă����ƁA�ڑ���̔����������Ȃ����Ƃ��Ɏ����Őؒf���邱�Ƃ��ł��܂��B
		@param timeout �^�C���A�E�g�~���b���ԁB0���w�肷��ƃ^�C���A�E�g���Ȃ��Ȃ�܂��B�����l��0�ł��B
		@endja
		*/
		void SetTimeoutTime(VUint64 timeout);
		//!@ja ping�𑗐M����Ԋu��ݒ肵�܂��B @endja
		/*!@ja
		@param interval �Ԋu���~���b�Őݒ肵�܂��B0�ɂ���ƍX�V���Ȃ��Ȃ�܂��B
		@endja
		*/
		void SetPingIntervalTime(VUint64 interval);
        //!@ja �f�[�^���M @endja
        /*!@ja 
        �f�[�^�𑗐M����Ƃ��Ɏg���܂��Bsz�T�C�Y���̃f�[�^����M���ɓ��B����ƁA��M����Parsed���R�[������܂��B
        @param p ���M����f�[�^�ւ̃|�C���^
        @param sz ���M����f�[�^�̃o�C�g��
        @return ���������ꍇ��true�A���s�����ꍇ��false
        @endja
        */
        virtual bool Merge(const void *p,size_t sz);

		enum TERMINATE_REASON
		{
			//!@ja ���� @endja
			TERMINATE_REASON_NORMAL=0,
			//!@ja �s�� @endja
			TERMINATE_REASON_INVALID=1,
			//!@ja ���[�U�G���A @endja
			TERMINATE_REASON_USER,

			//!@ja �g���܂���B @endja
			TERMINATE_REASON_32BIT=0x7fffffff
		};

		//!@ja �ؒf @endja
		/*!@ja
		Session �N���X�� Close �֐��Ƃ͈قȂ�A������͈Ӑ}���Đؒf���Ă��邱�Ƃ��`���܂��B
		�ڑ��悪 VCE_CLOSE_REMOTE �̂Ƃ��A����I���Ȃ̂��s���I���Ȃ̂����f���邱�Ƃ��ł��܂��B
		�������A ForceClose �֐����g���Ɛڑ���ɓ`��邱�ƂȂ��ؒf����܂��B
		@param reason �l�� 1 �����ݒ�ł��܂��B�uTERMINATE_REASON_USER+n�v�̂悤�ɐݒ肷�邱�Ƃ��\�ł��B
		@sa Close
		@sa Terminated
		@endja
		*/
		void Terminate(unsigned int reason=TERMINATE_REASON_NORMAL);

		//!@ja �ؒf�����Ƃ��ɌĂ΂��R�[���o�b�N�֐� @endja
		/*!@ja
		�ؒf�����Ƃ��ɌĂ΂�܂��B���̌� Closed �֐����R�[������܂��B
		@param reason �ݒ肳�ꂽ�l�ł��B
		@endja
		*/
		virtual void Terminated(unsigned int reason);

		//!@ja DirectContact ��L���ɂ��܂��B @endja
		/*!@ja
		@param contactid ���ׂẴA�v���P�[�V�����̃Z�b�V�����ň�ӂȃ[���ȊO�̒l
		@param timeouttime ���M�p�P�b�g���^�C���A�E�g����܂ł̎���
		@param reliable true�ɂ���ƃ^�C���A�E�g�����Ƃ��Ɏ����đ����܂��B
		@param debug true�ɂ���ƃf�o�b�O���[�h�Ƃ��ē��삵�܂��B
		@endja
		*/
		bool DirectContactEnable(VUint32 contactid,VUint64 timeouttime=2000,bool reliable=true,bool debug=false);

		//!@ja DirectContact �𖳌��ɂ��܂��B @endja
		void DirectContactDisable();

		//!@ja ���̃p�P�b�g�̂��Đ��ݒ� @endja
		bool DirectContactSetNext(VUint32 contactid);

		//!@ja ��M�����p�P�b�g�̑��M�����擾 @endja
		VUint32 DirectContactGetLast();

		enum DIRECTCONTACT_PROGRESS
		{
			//!@ja ���M���� @endja
			DIRECTCONTACT_PROGRESS_SEND,
			//!@ja �đ��M���� @endja
			DIRECTCONTACT_PROGRESS_RESEND,
			//!@ja ���M�������� @endja
			DIRECTCONTACT_PROGRESS_COMPLETE,
			//!@ja �^�C���A�E�g���� @endja
			DIRECTCONTACT_PROGRESS_TIMEOUT
		};

		struct DirectContactPacketStatus
		{
			//!@ja �p�P�b�g�̈�ӂ� ID @endja
			VUint32 transactionID;
			//!@ja ���Đ� @endja
			VUint32 to;
			//!@ja ���M�����Ƃ��̎��� @endja
			VUint64 timestamp;
		};

		//!@ja DirectContact �ł̌o�߂��󂯎��R�[���o�b�N�֐� @endja
		virtual void DirectContactProgress(DIRECTCONTACT_PROGRESS progress,const DirectContactPacketStatus &status);

		/*!@ja
		DirectContact�@�\���L���ɂȂ��Ă��邩�ǂ������ׂ܂��B
		@return 0�ȊO�ł���ΗL���ł��B�L���̏ꍇ�� contactID ��Ԃ��܂��B
		@endja
		*/
		VUint32 DirectContactIsEnable();

		struct DirectContactNodeStatus
		{
			VUint32 contactID;
			VUint32 hops;
			VUint64 lastResponseTime;
			VUint64 ping;
		};

		/*!@ja
		�ڑ���̏�Ԃ����ׂĎ擾���܂��B
		�f�o�b�O���[�h�ȊO�̂Ƃ��ɗ��p���Ă������N����܂���B
		@endja
		*/
		vce::g_vector<DirectContactNodeStatus> DirectContactGetNodeStatus();

		/*!@ja
		���M�\�Ȕ͈͂ł��ׂĂ� Ping �Ȃǂ𑗂� ��Ԃ��擾���܂��B
		��Ԃ̍X�V�͔񓯊��ōs���A�܂���������悤�ȋ�؂���Ȃ����߂��΂炭���Ă���
		DirectContactGetNodeStatus �𗘗p���Ă��ꂼ��̏�Ԃ��擾���܂��B
		�f�o�b�O���[�h�ȊO�̂Ƃ��ɗ��p���Ă������N����܂���B
		@endja
		*/
		void DirectContactUpdateNodeStatus();


		void*DirectContactInstance;
    };

	/*!@ja
	�����̃Z�b�V�������܂Ƃ߂�Ƃ��́A Codec �̑���ɂ��̃N���X���p�����A MPISession �ɓn���܂��B
	@endja
	*/
    class MPINode
        :public Codec
    {
        friend class MPISession;
	private:
		//�ȉ��̊֐��͎g���܂���BMPISession�Ŏg���Ă��������B
		void SetTimeoutTime(VUint64 timeout);
		void SetPingIntervalTime(VUint64 interval);
		void DirectContactEnable(VUint32 contactid,VUint64 timeouttime,bool reliable);
    public:
		//!@ja �R���X�g���N�^ @endja
		/*!@ja 
		@param number �m�[�h�ŗL�̔ԍ���U��܂��B
		@endja
		*/
        MPINode(unsigned char number);
		MPINode &operator =(MPINode &);
		//!@ja �e @endja
        class MPISession *pnode;
		//!@ja ���ʎq @endja
        const unsigned char MPINumber;
    public:
		//!@ja ���M�֐� @endja
		/*!@ja 
		Codec��Merge�Ɠ����悤�Ɏg���܂��B
		@sa Codec::Merge
		@endja
		*/
        bool Merge(const void *p,size_t sz);
    };

	typedef std::map<unsigned char,MPINode*> MPIMap;
	typedef std::map<unsigned char,MPINode*>::iterator MPIMapItr;
    //!@ja �����Z�b�V�������܂Ƃ߂�N���X @endja
    class MPISession
        :public Codec
    {
		//!@ja �m�[�h�ꗗ @endja
        MPIMap node;
    protected:
		//!@ja Codec��Parsed������ŃI�[�o�[���C�h���Ă��܂� @endja
        void Parsed(const VUint8 *p,size_t sz);
    public:
//      int Merge(const void *p,size_t sz,unsigned char MPInum);
		//!@ja �m�[�h�o�^ @endja
		/*!@ja 
		@param node �o�^����m�[�h
		@return ���������ꍇ��true�B
		@endja
		*/
        bool Register(MPINode *node);
		//!@ja �擪�̃m�[�h�擾 @endja
        /*!@ja
		�m�[�h�̐擪�̃C�e���[�^���擾���܂��B
		@endja
		*/
		MPIMapItr GetNodeBegin() { return node.begin(); }
		//!@ja �����̃m�[�h�擾 @endja
        /*!@ja
		�m�[�h�̍Ō�̃C�e���[�^���擾���܂��B
		@endja
		*/
		MPIMapItr GetNodeEnd() { return node.end(); }
		//!@ja ����̃N���X�̃m�[�h���擾 @endja
        /*!@ja 
		�擾�������m�[�h�̃N���X���������ɓ���Ďg���܂��B
		@endja
		*/
        template <typename N>N*GetNode(){for(MPIMapItr i=node.begin();i!=node.end();i++)if(dynamic_cast<N*>(i->second))return dynamic_cast<N*>(i->second);return NULL;}
    protected://wrapper
        void Attached();//�Snode��Attached���Ă�
        void Connected();//�ȉ�����
        void Closed(VCE_CLOSEREASON type);
        void Detached();
        void Exception(VCE_EXCEPTION type);
        void Think();
    };

	//!@ja HTTP�w�b�_�[�p��` @endja
	typedef std::map<std::string,std::string> HTTPHeader;
	//!@ja URI�p�����[�^�p��` @endja
	typedef std::map<std::string,std::string> URIParam;

	//!@ja �P����HTTP�T�[�o�p���X�i�[ @endja
	class HTTPServerListener
		:public Listener
	{
	public:
		typedef std::vector<std::string> PasswordList;
		struct authinfo
		{
			PasswordList passwords;
			std::string realm;
		};
		typedef std::map<std::string,authinfo> AuthList;
		typedef std::map<std::string,authinfo>::iterator AuthListItr;
	private:
		//!@ja Basic�F�؂̃��[�U�Ǘ����X�g�B�f�B���N�g�������L�[�Ƃ��A�p�X���[�h�̃��X�g��l�Ƃ���map�ł��B @endja
		AuthList authinfos;
	public:
		HTTPServerListener();
		HTTPServerListener&operator=(HTTPServerListener&){return*this;}

		/*!@ja
		�����郆�[�U��ǉ����܂��B��̃f�B���N�g���ɕ����̃��[�U��ݒ肷�邱�Ƃ��\�ł��B
		@param dir �f�B���N�g��
		@param user ���[�U��
		@param password �p�X���[�h
		@endja
		*/
		void AddUser(const std::string& dir, const std::string& user, const std::string& password);
		/*!@ja
		�F�؃_�C�A���O�ɏo��������ݒ肵�܂��B
		AddUser�̎��s��ɐݒ肵�܂��B
		@param dir �f�B���N�g��
		@param realm ����
		@endja
		*/
		void SetRealm(const std::string& dir, const std::string& realm);
		/*!@ja
        ���N�G�X�g���F�؍ς݂��`�F�b�N���܂��B
        @param header ���N�G�X�g�̃w�b�_��n���܂��B
		@return �F�؍ς݂Ȃ�true�A����ȊO�Ȃ�false�B
		@endja
		*/
		bool CheckAuth(const std::string& URI, const HTTPHeader &header);
		//!@ja �w�肳�ꂽ�f�B���N�g���̔F�؂̐������擾���܂��B @endja
		std::string GetRealm(const std::string& URI);
		//!@ja URI����f�B���N�g�������𔲂��o�� @endja
		/*!@ja
		www.hoge/aho/foo/index.html�̂悤��URI��^����ꂽ��hoge/aho/foo�Ahoge/aho�Ahoge�ƎO�̕����񂪕Ԃ�܂��B
		�Ō�Ɋg���q���Ȃ��ꍇ�A�������f�B���N�g���Ƃ��ĔF������܂��B
		@endja
		*/
		std::vector<std::string> GetDir(const std::string& URI);
	};

	//!@ja ����HTTP�Z�b�V�����J�n�N���X @endja
    /*!@ja 
    �ڑ��������Ƃ��Ɏ����ŃT�[�o�pHTTP�Z�b�V�����𐶐����A�Ԃ����߂̃e���v���[�g�N���X�ł��B
    ���̃N���X���烊�X�i�[�𐶐����AListen�֐��Ɉ����Ƃ��ēn�������œ��삵�܂��B
	@sa net::HTTPServerListening
    @endja
    */
    template <class T>class AutoHTTPAccepter:public HTTPServerListener
    {
		//!@ja �ڑ��������Ă��邩 @endja
        bool enable;
		//!@ja �ő哯���ڑ��� @endja
        const unsigned int maxconnections;
    protected:
		//!@ja Accepter @endja
        virtual Session *Accepter(VUint32,VUint16){
            if(maxconnections&&GetChildren()>=maxconnections)
                return NULL;
            return enable?new T:NULL;}
    public:
		//!@ja �R���X�g���N�^ @endja
		/*!@ja 
		@param maxconn �ő哯���ڑ���
		@endja
		*/
        AutoHTTPAccepter(int maxconn=0):maxconnections(maxconn){
            enable=true;}
        //!@ja ����̐ڑ��������܂��B @endja
        void Enable(){
            enable=true;}
        //!@ja ����̐ڑ������ۂ��܂��B @endja
        void Disable(){
            enable=false;}
		AutoHTTPAccepter&operator=(AutoHTTPAccepter&){return*this;}
    };

	//!@ja HTTP�T�[�o�̃��X�|���X�̕����R�[�h�̎�� @endja
	enum HTTP_CHARSET
	{
		HTTP_CHARSET_UTF_8 = 0,
		HTTP_CHARSET_SJIS = 1,
		HTTP_CHARSET_NONE = 2,
		HTTP_CHARSET_NUM = HTTP_CHARSET_NONE
	};
	//!@ja HTTP�T�[�o�̃��X�|���X��Content-Type�̎�� @endja
	enum HTTP_CONTENTTYPE
	{
		HTTP_CONTENTTYPE_HTML = 0,
		HTTP_CONTENTTYPE_TEXT = 1,
		HTTP_CONTENTTYPE_PLAIN = 2,
		HTTP_CONTENTTYPE_XML = 3,
		HTTP_CONTENTTYPE_CSS = 4,
		HTTP_CONTENTTYPE_NUM = HTTP_CONTENTTYPE_CSS
	};

	//!@ja �P����HTTP�T�[�o @endja
    /*!@ja 
	�ȒP��HTTP�T�[�o�����Ƃ��Ɏg���܂��B���N�G�X�g�ɑ΂��č����ȃ��X�|���X���ł��܂��BHTTPS�ɂ͑Ή����Ă��܂���B
	Basic�F�؂̃`�F�b�N��Recv�֐����Ŏ����I�ɂ����Ȃ��Ă���̂ŁA���̊֐����I�[�o���C�h���Ȃ�����C�ɂ���K�v�͂���܂���B
    @endja
    */
    class HTTPServerSession
        :public Session
    {
		//!@ja Recv�̃I�[�o�[���C�h @endja
		/*!@ja
		��M�������N�G�X�g��HTTP�w�b�_�����o�����肵�Ă��܂��B���̒���Basic�F�؂̃`�F�b�N���s���Ă��܂��B
		@sa Session::Recv
		@endja
		*/
        size_t Recv(const vce::VUint8 *p,size_t sz);
    protected:
		static const char *charset[];
		/*!@ja
        �N���C�A���g���烊�N�G�X�g����M����ƌĂ΂�܂��B
		1�̃��N�G�X�g�ɑ΂��ĕK��1�̃��X�|���X��Ԃ��K�v������܂��B
		�f�t�H���g�̓���ł́Aroot�ȉ�����URI�ŗ^����ꂽ�t�@�C����T���ĕԐM���܂��B
		@param method HTTP���\�b�h�B"GET"��"POST"�ȂǁB
		@param URI URI�B�Ώۃ��\�[�X�ւ̃A�h���X���w�肵�܂��B
		@param header HTTP�w�b�_�̖��O���L�[�Ƃ��A�Ή�������e��l�Ƃ���map�B
		@param body �f�[�^�{�́BGET��HEAD���\�b�h�̏ꍇ�͋�ł��B
		@param body_len �f�[�^�{�̂̃o�C�g���B
		@return ����ɕԐM�����Ƃ���true�A�ؒf����Ƃ���false�B
		@endja
		*/
        virtual bool Request(const std::string& method,const std::string& URI,const HTTPHeader& header,const char *body,size_t body_len);
		/*!@ja
        �N���C�A���g�փ��X�|���X��Ԃ��Ƃ��ɌĂт܂��B
		@param status HTTP���X�|���X�R�[�h�B
		@param header �w�b�_�ɕK�v�ȗv�f�����܂��B"Content-Length"�͎����I�ɐݒ肳��܂��B
		@param body �f�[�^�{�́BHEAD���\�b�h�̏ꍇ�͋�AGET��POST�̂Ƃ��͓��e��Ԃ��悤�ɂ��܂��B
		@param body_len �f�[�^�{�̂̃o�C�g��
		@return �Ȃ�
		@endja
		*/
        void Response(int status,const HTTPHeader& header,const char *body,size_t body_len);
		//!@ja �g���q���画�f����Content-Type�̕������Ԃ� @endja
		/*!@ja 
		�g���q���画�f����"text/html; charset=UTF-8"�̂悤��HTTP�w�b�_�p�̕������Ԃ��܂��B�f�t�H���g��text/html�ł��B
		@endja
		*/
		std::string GetContentTypeString(const std::string& URI, HTTP_CHARSET cs);
		std::string GetContentTypeString(HTTP_CONTENTTYPE type, HTTP_CHARSET cs);
		/*!@ja 
		�w�b�_���烆�[�U�������o���܂��B������Ȃ��ꍇ�͋��string���Ԃ�܂��B
		@param header ���N�G�X�g�̃w�b�_
		@return ���[�U��
		@endja
		*/
		std::string GetAuthUser(const HTTPHeader &header);
		/*!@ja 
		�F�؂����s�����Ƃ��̃��X�|���X��Ԃ��܂��B������Response���Ă΂�܂��B
		@param realm �F�؂ɂ��Ă̐���
		@endja
		*/
		void ResponseAuth(const std::string& realm);
		/*!@ja 
        ���N�G�X�g���F�؍ς݂��`�F�b�N���܂��B
		�F�؂����s�Ȃ�A�����I��ResponseAuth���ĂԂ��ƂŃ��X�|���X��Ԃ��܂��B
        @param header ���N�G�X�g�̃w�b�_�����̂܂ܓn���܂��B
		@return �F�؍ς݂Ȃ�true�A����ȊO�Ȃ�false�B
		@endja
		*/
		bool CheckAuth(const std::string& URI, const HTTPHeader &header);
	public:
		//!@ja �t�@�C����T�����̃��[�g�f�B���N�g�� @endja
		std::string root;
		//!@ja �y�[�W���N�G�X�g���̕����R�[�h @endja
		HTTP_CHARSET code;
		typedef std::map<std::string, std::string> ContentTypeMap;
		//!@ja �g���q�ƑΉ�����Content-Type�̃��X�g @endja
		/*!@ja
		�ŏ�����"cgi�Ahtm�Ahtml�Atxt"��"text/html"�A"js"��"text/plain"�A"css"��"text/css"�A"xml�Axsl"��"text/xml"�A
		"jpg"��"image/jpeg"�A"png"��"image/png"�A"gif"��"image/gif"�œo�^����Ă��܂��B�K�X�A�ǉ��ύX���Ă��������B
		@endja
		*/
		ContentTypeMap contenttype;

		HTTPServerSession();
		~HTTPServerSession(){}
		HTTPServerSession&operator=(HTTPServerSession&){return*this;}
    };

	//!@ja �P����HTTP�N���C�A���g�p�Z�b�V���� @endja
	class HTTPClientSession
		:public Session
	{
		std::string lastrequest;
		size_t Recv(const vce::VUint8 *p,size_t sz);
	public:
		//!@ja HTTP�T�[�o�Ƀ��N�G�X�g���o���܂� @endja
		/*!@ja
		@param method HTTP���\�b�h�B"GET"��"POST"�ȂǁB
		@param URI ���[�g�Ȃ�"/"���w�肵�܂��BURI�Ɏg���Ȃ�������encodeURI���Ă��������B
		@param header �w�b�_�ɕK�v�ȗv�f�����܂��B"Host"��"Content-Length"�͎����I�ɐݒ肳��܂��B
		@param body �f�[�^�{�́BPOST�̏ꍇ�ɕK�v�Ȃ����܂��B�s�v�ł����NULL��ݒ肵�܂��B
		@param body_len �f�[�^�{�̂̃o�C�g��
		@endja
		*/
		bool Request(const std::string& method,const std::string& URI,const HTTPHeader& header,const char *body,size_t body_len);
	protected:
		//!@ja ���X�|���X�R�[���o�b�N�֐� @endja
		/*!@ja
		���X�|���X����������Ƃ��ɃI�[�o�[���C�h���܂��B
		@param status HTTP���X�|���X�R�[�h
		@param header HTTP�w�b�_��map
		@param body �f�[�^�{��
		@param body_len �f�[�^�{�̂̃o�C�g��
		@return true�Ȃ�ڑ��ێ��Afalse�Ȃ�ؒf�B
		@endja
		*/
		virtual bool Response(int status,const HTTPHeader& header,const char *body,size_t body_len)=0;
		void Closed(VCE_CLOSEREASON type);
	};

	class MonitorConsole;
	typedef std::vector<vce::MonitorConsole*>::iterator MonitorConsoleListItr;
	class MonitorCustomPage;
	typedef std::vector<vce::MonitorCustomPage*>::iterator MonitorCustomPageListItr;

	//!@ja ���j�^�[�p���X�i�[ @endja
	/*!@ja
	���̃N���X�̐�����MonitorListenerCreate�֐�����݂̂����Ȃ��Ă��������B
	�C���^�[�t�F�C�X�݂̂ł����A���ꂼ��̊֐������[�U�[��`����K�v�͂���܂���B
	@endja
	*/
	class MonitorListener
		:public HTTPServerListener
	{
	protected:
		MonitorListener();

	public:
		virtual ~MonitorListener();
		//!@ja  MonitorConsole��o�^ @endja
		/*!@ja 
		�o�^�͕K�{�ł����AMonitorConsole�̃R���X�g���N�^�ŌĂ΂��̂ŁA�����ŌĂяo���K�v�͂���܂���B
		@endja
		*/
		virtual void RegisterConsole(MonitorConsole* con)=0;
		//!@ja  MonitorConsole��o�^����폜 @endja
		/*!@ja
		MonitorConsole�̃f�X�g���N�^�ŌĂ΂��̂ŁA�����ŌĂяo���K�v�͂���܂���B
		@endja
		*/
		virtual void DeregisterConsole(MonitorConsole* con)=0;
		//!@ja MonitorCustom��o�^ @endja
		/*!@ja
		�o�^�͕K�{�ł����AMonitorCustom�̃R���X�g���N�^�ŌĂ΂��̂ŁA�����ŌĂяo���K�v�͂���܂���B
		@endja
		*/
		virtual void RegisterCustom(MonitorCustomPage* cus)=0;
		//!@ja MonitorCustom��o�^����폜 @endja
		/*!@ja
		MonitorCustom�̃f�X�g���N�^�ŌĂ΂��̂ŁA�����ŌĂяo���K�v�͂���܂���B
		@endja
		*/
		virtual void DeregisterCustom(MonitorCustomPage* cus)=0;

		//!@ja id����MonitorConsole�����o�� @endja
		virtual MonitorConsole *GetConsoleFromID(vce::VUint32 consoleID)=0;
		//!@ja id����MonitorCustomPage�����o�� @endja
		virtual MonitorCustomPage *GetCustomPageFromID(vce::VUint32 customID)=0;

		//!@ja name����MonitorConsole�����o�� @endja
		virtual vce::MonitorConsole *GetConsoleFromName(const std::string& name)=0;
		//!@ja name����MonitorCustomPage�����o�� @endja
		virtual vce::MonitorCustomPage *GetCustomPageFromName(const std::string& name)=0;

		//!@ja iterator.begin @endja
		virtual MonitorConsoleListItr GetConsoleBegin()=0;
		//!@ja iterator.end @endja
		virtual MonitorConsoleListItr GetConsoleEnd()=0;
		//!@ja iterator.begin @endja
		virtual MonitorCustomPageListItr GetCustomBegin()=0;
		//!@ja iterator.end @endja
		virtual MonitorCustomPageListItr GetCustomEnd()=0;

		//!@ja �e���v���[�g�ǂݍ��� @endja
		/*!@ja
		���ꂼ��̃y�[�W�̃e���v���[�g���J�����g�f�B���N�g���̈ȉ��̃t�@�C������ǂݍ��݂܂�:
		index.html�Astatus.html�Aconsole.html�Acustom.html�Adummy.html�B
		@endja
		*/
		virtual void ReadTemplate()=0;
	};

    //!@ja Monitor�R���\�[�� @endja
    /*!@ja 
    ���j�^�[�Ɗ֘A���Ă��낢��g�����߂̃R���\�[���ł��B
	MonitorListener�ɓo�^����Ă���R���\�[����web�u���E�U����g�p�ł��܂��B
	�R���X�g���N�^�œo�^�A�f�X�g���N�^�œo�^����������܂��B
	�C�ӂ̃^�C�~���O�œo�^�������������ꍇ��Detach���g�p���Ă��������B
    @endja
    */
	class MonitorConsole
    {
		//!@ja �R���\�[��ID @endja
		vce::VUint32 consoleID;
		vce::MonitorListener* ml;
		//!@ja �R���\�[���� @endja
		std::string cname;
		//!@ja �y�[�W�����M����鎞�̕����R�[�h @endja
		/*!@ja
		HTTPServerSession::code���D�悳��܂��B
		@endja
		*/
		HTTP_CHARSET code;
    public:
		//!@ja �R���X�g���N�^ @endja
		MonitorConsole(vce::MonitorListener *parent, const std::string& name = "");
		MonitorConsole&operator=(MonitorConsole&);
		MonitorConsole(MonitorConsole&);
		//!@ja �f�X�g���N�^ @endja
        virtual ~MonitorConsole();
		//!@ja ID�擾 @endja
		inline vce::VUint32 GetID()const{return consoleID;}
		//!@ja �R�}���h���󂯎�����Ƃ��ɌĂ΂�܂��B @endja
		/*!@ja
		cmd��web�u���E�U�œ��͂��ꂽ������ɂȂ�܂��B
		@endja
		*/
        virtual void CommandRequest(const std::string& cmd);
		//!@ja MonitorListener����؂藣���܂��B @endja
		void Detach();
		//!@ja ���O���擾 @endja
		std::string GetName(){return cname;}
    public:
        //!@ja ���O�o�͗p�̃X�g���[�� @endja
        std::stringstream clog;
    };

	//!@ja Monitor�_�~�[�v���g�R�� @endja
	/*!@ja 
	���d�p����gen�̐ݒ�ɂ��_�~�[�v���g�R���𑗐M�ł��܂��B
	���̃N���X��gen�ɂ���Đ������ꂽ�Z�b�V�����N���X�Ɍp�������Ďg���܂��B
	�ڂ����̓`���[�g���A�����Q�Ƃ��Ă��������B
	@endja
	*/
   class MonitorDummyProtocol
    {
		//!@ja �_�~�[�v���g�R��ID @endja
		vce::VUint32 dummyID;
		//!@ja �^�u�Ȃǂɕ\������閼�O @endja
		std::string dpname;
    protected:
		//!@ja �R���X�g���N�^ @endja
		MonitorDummyProtocol(const std::string& name = "");
		MonitorDummyProtocol&operator=(MonitorDummyProtocol&);
		MonitorDummyProtocol(MonitorDummyProtocol&);
		//!@ja �f�X�g���N�^ @endja
        virtual ~MonitorDummyProtocol();
    public:
		//!@ja ID�擾 @endja
		inline vce::VUint32 GetID() const { return dummyID; }
		//!@ja �_�~�[�v���g�R���̖��O��ݒ� @endja
		void SetName(const std::string& name) { dpname = name; }
		//!@ja �_�~�[�v���g�R���̖��O���擾 @endja
		inline std::string GetName() const { return dpname; }
		//!@ja HTTP��POST��������Ă΂�� @endja
		/*!@ja
		��`�͎����ŏ����K�v������܂����Agen�Ő������ꂽ�Z�b�V������PostRequest�֐����ĂԂ悤�ɂ��邾���ōς݂܂��B
		�ڂ����̓`���[�g���A�����Q�Ƃ��Ă��������B
		@endja
		*/
        virtual void Post(const char *post)=0;
        //!@ja �_�~�[�v���g�R���y�[�W��HTML @endja
		/*!@ja
		���̒��g��gen���������Ă���܂��B
		@endja
		*/
        std::stringstream html;
		//!@ja �_�~�[�v���g�R���y�[�W��JavaScript @endja
		/*!@ja
		���̒��g��gen���������Ă���܂��B
		@endja
		*/
		std::stringstream js;

		//!@ja ��M�����v���g�R���ЂƂ��̃f�[�^�̕ۑ��p�\���� @endja
		struct ProtocolData
		{
			vce::VUint32 time;
			vce::g_string name;
			vce::g_vector<vce::VUint8> data;
		};
		typedef std::vector<ProtocolData> ProtocolDataList;
		typedef std::vector<ProtocolData>::iterator ProtocolDataListItr;
		//!@ja ��M�����v���g�R���̃��X�g @endja
		/*!@ja
		�v���g�R������M����Ƃ��̃��X�g�ɒǉ�����܂��B
		@endja
		*/
		ProtocolDataList recordproto;
		//!@ja ��M�����v���g�R���̃��X�g��XML�`���ɂ��� @endja
		/*!@ja
          �v���g�R�����Ǝ�M���Ԃ̃��X�g���o�͂��܂��B
		@param out ���������XML�f�[�^
		@endja
		*/
		void GetRecordProtocolXML(std::stringstream &out);
		//!@ja ��M�����v���g�R�����Č����� @endja
		/*!@ja
		��`�͎����ŏ����K�v������܂����Agen�Ő������ꂽ�Z�b�V������Replay�֐����ĂԂ悤�ɂ��邾���ōς݂܂��B
		�ڂ����̓`���[�g���A�����Q�Ƃ��Ă��������B
		@endja
		*/
		virtual void ReplayRecords(VUint32 index);
	};

	//!@ja Monitor�J�X�^���y�[�W @endja
	/*!@ja
	�A�v���P�[�V�����Ǝ��̂؁[�W���쐬�ł��܂��B
	MonitorListener�ɓo�^����Ă���R���\�[����web�u���E�U����g�p�ł��܂��B
	�R���X�g���N�^�œo�^�A�f�X�g���N�^�œo�^����������܂��B
	�C�ӂ̃^�C�~���O�œo�^�������������ꍇ��Detach���g�p���Ă��������B
	@endja
	*/
    class MonitorCustomPage
    {
		vce::VUint32 customID;
		vce::MonitorListener* ml;
		//!@ja �^�u�Ȃǂɕ\�����閼�O @endja
		std::string cpname;
	public:
		//!@ja �J�X�^���y�[�W��HTML @endja
        std::stringstream html;
		//!@ja �J�X�^���y�[�W���̎����X�V�Ԋu(�~���b�P��) @endja
		vce::VUint32 interval;
		//!@ja �J�X�^���y�[�W���̎����X�V����^�O�̗v�f�� @endja
        std::string element;
		//!@ja �u���E�U�̃y�[�W�L���b�V���𖳌��ɂ��邩�ǂ����B @endja
		bool nocache;
		//!@ja �y�[�W�����M����鎞�̕����R�[�h @endja
		/*!@ja
		HTTPServerSession::code���D�悳��܂��B
		@endja
		*/
		HTTP_CHARSET code;
		//!@ja �y�[�W�����M����鎞��Content-Type @endja
		/*!@ja
		HTTPServerSession::type���D�悳��܂��B
		@endja
		*/
		HTTP_CONTENTTYPE type;

		//!@ja �R���X�g���N�^ @endja
		MonitorCustomPage(vce::MonitorListener *owner, const std::string& name = "");
		MonitorCustomPage&operator=(MonitorCustomPage&);
		MonitorCustomPage(MonitorCustomPage&);
		//!@ja �f�X�g���N�^ @endja
        virtual ~MonitorCustomPage();
		//!@ja ID���擾 @endja
		inline vce::VUint32 GetID() const { return customID; }
		//!@ja �J�X�^���y�[�W�̖��O���擾 @endja
		inline std::string GetName() const { return cpname; }
		//!@ja MonitorListener����؂藣�� @endja
		void Detach();
		//!@ja �y�[�W�Ǝ���POST���� @endja
		/*!@ja
		web�u���E�U�ł��炩���ߗp�ӂ���Ă���JavaScript(SendCustomProtocolRequest)���Ă񂾂Ƃ��ɌĂяo�����֐��B
		@param post web�u���E�U���瑗���Ă������N�G�X�g�̃f�[�^�{�́B
		@endja
		*/
        virtual void Post(const char *post)=0;
		//!@ja �y�[�W�����M����钼�O�ɌĂ΂��܂��B @endja
		/*!@ja
		���m�ɂ́A�^�u�̒��g��Ԃ����O�ɌĂ΂��R�[���o�b�N�֐��B
		���N�G�X�g�ɂ���ăy�[�W�̓��e��ύX���������́A���̊֐��̒���html��ύX���܂��B
		������Ǝ��Ɏg�������Ƃ���JavaScript�����������܂��B�f�t�H���g����HTTP���\�b�h��"GET"�Aparam�ɂ�ID�������Ă��܂��B
		@param method web�u���E�U���瑗���Ă������N�G�X�g��HTTP���\�b�h�B
		@param param URI�̃N�G���B
		@param body web�u���E�U���瑗���Ă������N�G�X�g�̃f�[�^�{�́B
		@endja
		*/
		virtual void PreContentSend(std::string method, vce::URIParam param, std::string body)=0;
		//!@ja �J�X�^���y�[�W����Ajax�I�Ȃ��Ƃ����鎞�ɌĂ΂�܂��B @endja
		/*!@ja
		interval�����o�Ɏw�肳�ꂽ�Ԋu�ŃT�[�o����PreContentPartSend�֐��̖߂�l���擾���Aelement�����o��id�Ɏ���HTML�v�f���ɕ\�����܂��Binterval��0�̏ꍇ�͉������܂���B
		@param body web�u���E�U���瑗���Ă������N�G�X�g�̃f�[�^�{�́B
		@return ���̖߂�l�̕����񂪑��M����܂��B
		@endja
		*/
		virtual std::string PreContentPartSend(std::string body)=0;
	};

    //!@ja VCE���C�u�����̏����� @endja
    /*!@ja 
	 VCE�S�̂ŋ��L���鎑�������������܂��B
	 VCECreate��VCE�I�u�W�F�N�g�����O�ɌĂяo���܂��B
	 VCE�̗��p���I��������VCEFinalize���Ăяo���܂��B
	 @sa vce::VCEFinalize
	 @endja
	 */
	bool VCEInitialize();

    //!@ja VCE���C�u�����̏I�� @endja
    /*!@ja 
	 vce::VCEInitialize�ŏ������������L������������܂��B
	 �ʏ�̓v���O�����̏I�����ɌĂяo���܂��B
	 @sa vce::VCEInitialize
	 @endja
	 */
	void VCEFinalize();

    //!@ja VCE�I�u�W�F�N�g�̐��� @endja
    /*!@ja 
    ���̊֐��ł܂�VCE�I�u�W�F�N�g�������API���g����悤�ɂ���K�v������܂��B������邱�Ƃ��ł��܂��B
	�����Ƀ��O���Z�b�g���邱�Ƃ��ł��܂��B
	@sa vce::SetLogger
    @endja
    */
    VCE *VCECreate(std::ostream *target=NULL,int loglevel=VCE_LOGLEVEL_LOW,MemoryAllocator *ma=&defaultallocator);
    //!@ja VCE�I�u�W�F�N�g�̍폜 @endja
    /*!@ja 
    VCECreate�ō��ꂽ�I�u�W�F�N�g�͕K�����̊֐��ō폜���Ȃ���΂Ȃ�܂���B�����������e��㏈�������܂��B
    @endja
    */
    void VCEDelete(VCE *n);

	//!@ja MonitorListener�I�u�W�F�N�g�̐��� @endja
	/*!@ja
	MonitorListener�̍쐬�͕K�����̊֐���p���Ă��������B
	@endja
	*/
	MonitorListener *MonitorListenerCreate();
	//!@ja MonitorListener�I�u�W�F�N�g�̍폜 @endja
	/*!@ja
	MonitorListener�̍폜�͕K�����̊֐���p���Ă��������B
	@endja
	*/
	void MonitorListenerDelete(MonitorListener *ml);


	//!@ja ���[�J���l�b�g���[�N���Ƀ}���`�L���X�g����N���X @endja
	class Finder
		:public Base
	{
		//!@ja �O���[�vID(�}���`�L���X�gIP�A�h���X) @endja
		unsigned int group;
	public:
		//!@ja �R���X�g���N�^ @endja
		/*!@ja
		���[�J���l�b�g���[�N���̑��̃A�v���P�[�V�������������܂��B
		@param group �����Ώۂ̃O���[�vID�B0xE0000001����0xEFFFFFFF�͈͓̔��ɐݒ肵�Ȃ���΂Ȃ�܂���B�}���`�L���X�gIP�A�h���X�̃O���[�v�ɂȂ�܂��B
		@endja
		*/
		Finder(unsigned int group);
		virtual ~Finder();
		//!@ja �O���[�vID���擾���܂��B @endja
		unsigned int GetGroupID();
		typedef std::map<std::string,std::string> Keywords;
		/*!@ja
		�L�[���[�h��ݒ肵�܂��B�����̕�������}�b�v�ł��܂����Akey��value���킹�čő�Ŗ�1000�o�C�g���܂łł��B
		@param key �ݒ肷��L�[���[�h�ւ̎Q�ƁB���̕ϐ��ɃL�[���[�h��ݒ肵�܂��B
		@endja
		*/
		virtual void Keyword(Keywords &key)=0;
        //!@ja ���������Ƃ��ɌĂ΂��R�[���o�b�N�֐� @endja
		/*!@ja
		�������g�Őݒ肵���L�[���[�h���q�b�g���܂��B
		���Ԋu�Ō������邽�߉��x�������L�[���[�h���q�b�g���܂��B
		@param key ���������L�[���[�h�ւ̎Q�ƁB
		@endja
		*/
		virtual void Find(Keywords &key)=0;
		//!@ja �����Ƃ��ɌĂ΂��R�[���o�b�N�֐� @endja
		virtual void Closed();

		//!@ja ���� @endja
		/*!@ja
		Close���邱�ƂŌ��������Ȃ��Ȃ�A����Finder�ɂ���������Ȃ��Ȃ�܂��B
		@endja
		*/
		bool Close();
	};

	/*!@ja
	TCP���X�i�[�����p�̃e���v���[�g�ł��B
	Session�������ɐڑ��������Z�b�V������ݒ肵�A
	�ڑ���U��������port���R���X�g���N�^�Őݒ肵�Ă��������B
	���[�J���l�b�g���[�N����TCPListenerFinder�𔭌������Connect���܂��B
	@endja
	*/
	template<typename Session>
	class TCPListenerFinder
		:public Finder
	{
		//!@ja �U������|�[�g�ԍ� @endja
		VUint16 port;
	protected:
		//!@ja ���X�i�[�U���p�̃L�[���[�h��ݒ肵�܂��B @endja
		/*!@ja
		@param key �L�[���[�h
		@endja
		*/
		void Keyword(Keywords &key)
		{
			//�����̃A�h���X�ƃ��X�j���O�|�[�g���L�[���[�h�ɐݒ肷��B
			unsigned int arry[4];
			size_t find=vce::GetSelfAddress(arry,4);
			// 0.0.0.0 �� 127.0.0.1 �ȊO�̍ŏ��Ɍ��ꂽ�l���g��
			for(int i =0; i < find; ++i)
			{
				if ((arry[i] > 0) && (arry[i] != 2130706433)) // not 127.0.0.1
				{
					key["address"] = vce::AddrToString(arry[i]);
					break;
				}
			}	
			key["port"]=ntoa(port);
		}
		//!@ja Find�̃I�[�o�[���C�h @endja
		/*!@ja
        ���X�i�[�����p�L�[������Connect���܂��B
        @param key �L�[���[�h
		@endja
		*/
		void Find(Keywords &key)
		{
			//�L�[���[�h���������L�邩�`�F�b�N
			if(!key["address"].empty()&&vce::atoi(key["port"]))
			{
				vce::VUint32 remoteaddr,addr=vce::StringToAddr(key["address"]);
				vce::VUint16 remoteport,port=vce::atoi(key["port"]);
				std::vector<vce::Session*> v;
				api->GetAllSession(v);
				//���łɓ������X�i�[�ւ̃Z�b�V�������������`�F�b�N
				for(size_t i=0;i<v.size();i++)
					if(v[i]->GetRemoteAddress(remoteaddr,remoteport)&&remoteaddr==addr&&remoteport==port)
						return;
				api->Connect(new Session,key["address"],port,2000);
			}
		}
	public:
		//!@ja �R���X�g���N�^ @endja
		/*!@ja
		�|�[�g�ƃO���[�v��ݒ肵�܂��B
		@param port �ڑ���U������TCP�|�[�g�ԍ�
		@sa Finder
		@endja
		*/
		TCPListenerFinder(VUint16 port,unsigned int group=0xeff0f1f2)
			:Finder(group)
		{
			this->port=port;
		}
	};

	//!@ja UPnP�|�[�g�ݒ�����邽�߂̃C���^�[�t�F�C�X�N���X @endja
	class UPnPStatus
	{
	public:
		//!@ja �|�[�g�}�b�s���O�̏�� @endja
		enum Status
		{
			//!@ja �s�� @endja
			UPnP_Unknown=0,
			//!@ja ���[�^������ @endja
			UPnP_SearchRouter,
			//!@ja ���[�^���擾�� @endja
			UPnP_GetRouterSpec,
			//!@ja �A�N�V���������� @endja
			UPnP_ActionWait,
			//!@ja ���������B���������삪���������Ƃ͌���܂���B @endja
			UPnP_Complete,
			//!@ja ���s @endja
			UPnP_Fail
		};
		//!@ja UPnP����̏�Ԃ��擾���܂��B @endja
		virtual Status GetUPnPStatus()=0;
		
		//!@ja UPnP�̑��삪��������܂ő҂��܂��B @endja
		/*!@ja
		��������������܂œ�����VCE::Poll���Ăяo���܂��B
		@param timeout �������^�C���A�E�g����܂ł̃~���b���ԁB
		@return UPnP_Complete�̏ꍇ��true�A����ȊO�̏ꍇ��false�B
		@endja
		*/
		virtual bool WaitComplete(VUint64 timeout=5000)=0;

		virtual ~UPnPStatus();
	};

	/*!@ja
	  UPnP�̑���̓X���b�h�Z�[�t�ł͂���܂���B
	  �����̃X���b�h�œ�����UPnP��API��p���邱�Ƃ͂ł��܂���B
	@param port IP�}�X�J���[�h��ݒ肷��|�[�g�ԍ�
	@param api VCE�I�u�W�F�N�g�ւ̃|�C���^
	@return UPnP��Ԏ擾�p�̃C���^�[�t�F�C�X
	@endja
	*/
	UPnPStatus* UPnPSetPortmapping(VUint16 port,VCE *api);
	/*!@ja
	  UPnP�̑���̓X���b�h�Z�[�t�ł͂���܂���B
	  �����̃X���b�h�œ�����UPnP��API��p���邱�Ƃ͂ł��܂���B
	@param port IP�}�X�J���[�h���폜����|�[�g�ԍ�
	@param api VCE�I�u�W�F�N�g�ւ̃|�C���^
	@return UPnP��Ԏ擾�p�̃C���^�[�t�F�C�X
	@endja
	*/
	UPnPStatus* UPnPDeletePortmapping(VUint16 port,VCE *api);
	//!@ja UPnP����I�u�W�F�N�g�̍폜 @endja
	/*!@ja
	���[�^������L�����Z������Ƃ���A����I����ɍ폜����K�v������܂��B
	UPnP�̑���̓X���b�h�Z�[�t�ł͂���܂���B
	�����̃X���b�h�œ�����UPnP��API��p���邱�Ƃ͂ł��܂���B
	@endja
	*/
	void DeleteUPnP(UPnPStatus *uppp);

	//!@ja QoSRequest�p @endja
	class UPnPQoSStatus
		:virtual public UPnPStatus
	{
	public:
		//!@ja QoS����p�n���h�����擾���� @endja
		/*!@ja
		Update�����Release����ɕK�v�ȃn���h���ԍ����擾���܂��B
		��Ԃ�UPnP_Complete�ȊO�̂Ƃ��Ɏ擾���悤�Ƃ���Ǝ��s���܂��B
		@param handle �n���h�����擾���邽�߂̕ϐ��ւ̎Q�ƁB
		@return ���������ꍇ��true�B
		@endja
		*/
		virtual bool GetTrafficHandle(int &handle)=0;
	};

	/*!@ja
	���[�^�̏��ш��ݒ肵�܂��B

	UPnP�̑���̓X���b�h�Z�[�t�ł͂���܂���B
	�����̃X���b�h�œ�����UPnP��API��p���邱�Ƃ͂ł��܂���B

	@param bandwidth ���ш��kbps�P�ʂŎw�肵�܂��B1Mbps�ł����1024�ł��B
	@param api VCE�I�u�W�F�N�g�ւ̃|�C���^�B
	@return UPnP��Ԏ擾�p�̃C���^�[�t�F�C�X�B
	@endja
	*/
	UPnPStatus* UPnPQoSSetUpstreamBandwidth(VUint32 bandwidth,VCE *api);

	/*!@ja
	�A�v���P�[�V��������L���������ш��ݒ肵�܂��B
	���[�^�ɂ���Đݒ�ł��鐔�����܂��Ă���̂ŁA�A�v���P�[�V�����I�����UPnPQoSReleaseTraffic�ŉ�����邩�AleaseTime��ݒ肵�Ă������Ƃ��]�܂����ł��B
	@param guaranteeBandwidth �ш���m�ۂ���p�[�Z���e�[�W�̐ݒ�B���v��100%�𒴂���Ǝ��s���܂��B�܂��A�K���ݒ肵���ш悪�ۏ؂����킯�ł͂���܂���B

	UPnP �̑���̓X���b�h�Z�[�t�ł͂���܂���B
	�����̃X���b�h�œ����� UPnP �� API ��p���邱�Ƃ͂ł��܂���B

	@param address �ΏۃA�h���X�B0���w�肷��ƃ��[�J���A�h���X���ݒ肳��܂��B
	@param portStart �Ώۃ|�[�g�͈͂̊J�n�B
	@param portEnd �Ώۃ|�[�g�͈͂̏I���BportStart�Ɠ����ɂ���ƈ�̃|�[�g�̂ݐݒ肳��܂��BportStart�����ɐݒ肷��Ƒ���Ɏ��s���܂��B
	@param prefixLength �A�h���X�}�X�N�r�b�g���B�ʏ��32���w�肵�܂��B
	@param isSource true�̏ꍇ�Aaddress��port��source�Ƃ��āAfalse�̏ꍇdestination�Ƃ��Đݒ肵�܂��B
	@param leaseTime �L���ɂ���b�P�ʂ̎��ԁB0�ɂ���Ɩ������ɂȂ�܂��B
	@param api VCE�I�u�W�F�N�g�ւ̃|�C���^�B
	@return UPnP��Ԏ擾�p�̃C���^�[�t�F�C�X�B
	@sa UPnPQoSReleaseTraffic
	@endja
	*/
	UPnPQoSStatus* UPnPQoSRequestTraffic(int guaranteeBandwidth,VUint32 address,VUint32 prefixLength,VUint16 portStart,VUint16 portEnd,bool isSource,VUint32 leaseTime,VCE *api);

	/*!@ja
	UPnPQoSRequestTraffic�Őݒ肵���l��ύX����Ƃ��Ɏg���܂��B

	UPnP �̑���̓X���b�h�Z�[�t�ł͂���܂���B
	�����̃X���b�h�œ�����UPnP��API��p���邱�Ƃ͂ł��܂���B

	@param handle UPnPQoSRequestTraffic�̕Ԃ�l�I�u�W�F�N�g����擾����GetTrafficHandle�̒l��ݒ肵�܂��B
	@sa UPnPQoSRequestTraffic
	@endja
	*/
	UPnPStatus* UPnPQoSUpdateTraffic(int handle,int guaranteeBandwidth,VUint32 address,VUint32 prefixLength,VUint16 portStart,VUint16 portEnd,bool isSource,VUint32 leaseTime,VCE *api);

	/*!@ja
	��L���Ă�����ш��������܂��B

	UPnP�̑���̓X���b�h�Z�[�t�ł͂���܂���B
	�����̃X���b�h�œ�����UPnP��API��p���邱�Ƃ͂ł��܂���B

	@param handle UPnPQoSRequestTraffic�̕Ԃ�l�I�u�W�F�N�g����擾����GetTrafficHandle�̒l��ݒ肵�܂��B
	@sa UPnPQoSRequestTraffic
	@endja
	*/
	UPnPStatus* UPnPQoSReleaseTraffic(int handle,VCE *api);
}

#endif//VCE_VCE2_H
