
#ifndef VCE_VCE2STREAM_H
#define VCE_VCE2STREAM_H

#include "vce2.h"

//#ifndef VCE_HAS_STREAM_EXT
//# error "Stream Extension is disable on this platform" 
//#endif

/**
 * @defgroup vcesx
 *
 * @ja 
 * �X�g���[���g�� API ���`���܂��B
 *
 * @todo ����
 * - �ړI
 * - �����Ǘ�
 *   - �A���P�[�^
 * @endja 
 *
 * @{
 */
namespace vce
{

	/* ------------------------------------------------------------
	 *
	 * �X�g���[���g���̃t���[�����[�N����`����C���^�[�t�F�C�X
	 *
	 * ------------------------------------------------------------ */

	/**
	 * @ja �Q�ƃJ�E���g�v���g�R���̃C���^�[�t�F�C�X
	 */
	class Any
	{
	public:
		virtual ~Any() {}

		/** @ja �Q�ƃJ�E���g���C���N�������g���܂� */
		virtual void Acquire() = 0;
		/** @ja �Q�ƃJ�E���g���f�N�������g���܂� */
		virtual void Release() = 0;
	};

	/**
	 * Unreact() ���������邽�߂̃C���^�[�t�F�C�X�ł��B
	 */
	template<class T>
	class Unreactable
	{
	public:
		virtual void Unreact(T* source) = 0;
	};

	/**
	 * @ja
	 * VCE �����̐ڑ��I�u�W�F�N�g�ɑ΂���C���^�[�t�F�C�X�B
	 * LoopReaction �ɑ΂��Č��J����܂��B 
	 *
	 * @see Reaction
	 * @endja
	 */
	class LoopBinding : virtual public Any
	{
	public:
		friend class LoopReaction;

		virtual ~LoopBinding() {}

		/**
		 * @ja
		 * �Y������ڑ��I�u�W�F�N�g��j�����܂��B
		 * Unbind() �̌Ăяo����� Reaction::Unound()���ʒm����A�ڑ��͔j������܂��B
		 * @endja
		 */
		virtual void Unbind() = 0;

		virtual void Unreact(LoopReaction* reac) = 0;
	};

	/**
	 * @ja
	 * ���O�o�͂��x�����邽�߂̃C���^�[�t�F�C�X�ł��B
	 * @endja
	 */
	class Inspectable
	{
	public:
		virtual ~Inspectable() {} // to supress compiler warning...
		/**
		 * ���O�s�̃w�b�_�ɑ}�����镶������o�͂��܂��B
		 * ���O���o�͂���I�u�W�F�N�g����肷��Œ���̏����o�͂���K�v������܂��B
		 *
		 * @param out �o�͐�X�g���[��
		 */
		virtual void InspectName(std::ostream& out) = 0;
	};

	/**
	 * @ja
	 * �X�g���[���ɑ΂�����o�̓o�b�t�@�B
	 * �X�g���[���g���ł́A�f�[�^�����𑗐M�o�b�t�@�Ǝ�M�o�b�t�@�̑΂Ƃ��ĕ\�����Ă��܂��B
	 * StreamBuffer �͑���M�����ꂩ�̃o�b�t�@�ɊY�����A StreamBinding �C���^�[�t�F�C�X��ʂ��Č��J����܂��B
	 *
	 * ��`����Ă��郁�\�b�h�S�Ă���ɗ��p�ł���킯�ł͂Ȃ��_�ɂ͒��ӂ��K�v�ł��B
	 * ���M�o�b�t�@����̓f�[�^��ǂݏo�����Ƃ��ł����A�܂���M�o�b�t�@�ɂ̓f�[�^���������ނ��Ƃ��ł��܂���B
	 *
	 * @see StreamBinding::GetRecvBuffer()
	 * @see StreamBinding::GetSendBuffer()
	 * @see MakeMemoryStreamBuffer()
	 * @endja
	 */
	class StreamBuffer : virtual public Any
	{
	public:
		virtual ~StreamBuffer() {}

		/**
		 * @ja
		 * �o�b�t�@�����Ƀf�[�^���������݂܂��BSession::Send() �ɊY�����܂��B
		 *
		 * @param p �������ރf�[�^�̐擪�o�C�g
		 * @param size �������ރf�[�^�̃o�C�g��
		 * @return �������݂ɐ�������� true�B �o�b�t�@�̗e�ʂ��O�a����� false 
		 * @endja
		 */
		virtual bool Write(const void *p,size_t size) = 0;

		/**
		 * @ja
		 * �o�b�t�@��擪����폜���܂��B
		 * �f�[�^��ǂݏo���ĉ��߂������ƁA���p�҂͎Q�Ƃ��Ȃ��f�[�^�� Shrink() �ō폜����K�v������܂��B
		 * Session::EraseReadbuf() �ɊY�����܂��B
		 *
		 * @param size �폜����o�C�g���B���݂���o�C�g���ȉ����w�肷��K�v������܂��B
		 * @endja
		 *
		 */
		virtual void Shrink(size_t size) = 0;

		/**
		 * @ja
		 * �o�b�t�@�e�ʂ�A�e�ʕs�����̐U�������w�肵�܂��B
		 * 
		 * @param size ���݂̃o�b�t�@�̃o�C�g��
		 * @param variable �o�b�t�@���ϒ��ɂ���Ȃ� true�B�ϒ��o�b�t�@�͗e�ʕs���ɃT�C�Y���g�傳��܂��B
		 * @param capacity �ϒ��o�b�t�@�̍ő�o�C�g��
		 * @return �ύX�Ɏ��s����� false
		 * @todo �ǂ�ȏꍇ�Ƀo�b�t�@�T�C�Y�ύX�͎��s����̂��H
		 * @endja
		 */
		virtual bool SetSize(size_t size,bool variable,size_t capacity) = 0;

		/**
		 * @ja
		 * �o�b�t�@�����̗L���ȃo�C�g����擾���܂��B
		 * �擾�����o�C�g������������邱�Ƃ͂ł��܂���B�ǂݏo���ړI�ɂ̂ݗ��p�ł��܂��B
		 * Session::GetReadbuf() �ɊY�����܂��B
		 *
		 * @param size �擾�����o�C�g��̗L���o�C�g���ł�
		 * @return �o�C�g��ւ̃|�C���^�ł��B
		 * @endja
		 */
		virtual const unsigned char*Peek(size_t &size) const = 0;
		
		template<class T>
		const T* PeekAs(size_t* size) const
		{ 
			size_t bytes = 0;
			const T* ret = reinterpret_cast<const T*>(Peek(bytes));
			*size = bytes/sizeof(T);
			return ret;
		}

		/** @ja @return �o�b�t�@�ɏ������݉\�ȃo�C�g�� */
		virtual size_t GetAvailable() const = 0;
		/** @ja @return �o�b�t�@��������킷�o�C�g�� */
		virtual size_t GetSize() const = 0;
		/** @ja @return �ϒ��o�b�t�@�̍ő�e�� */
		virtual size_t GetCapacity() const = 0;
		/** @ja @return �ϒ��o�b�t�@�Ȃ��[�� */
		virtual int GetVariable() const = 0;
		/** @ja @return �o�b�t�@�ɏ������܂�Ă���f�[�^�̃o�C�g�� */
		virtual size_t GetUsed() const = 0;
	};

	/**
	 * @ja
	 *
	 * �I�u�W�F�N�g�� "�֘A�Â���" ���߂̃C���^�[�t�F�C�X�ł��B
	 * ������ʒm���邽�߂ɗp���܂��B Any �ƈق�A�o�^�����I�u�W�F�N�g��
	 * Bound() �� Unbound() ����x�����Ăяo���܂��B
	 *
	 * @param B �Ή��Â���I�u�W�F�N�g (binding) �̌^
	 * @endja
	 */
	template<class B>
	class Reaction
	{
	public:
		virtual ~Reaction() {}
		/**
		 * @ja
		 * B �̃C���X�^���X���֘A�Â���炽���ɒʒm����܂��B
		 * @param source �֘A�t�����ꂽ�I�u�W�F�N�g
		 * @endja
		 */
		virtual void Bound(B* source) = 0;
		/**
		 * @ja
		 * B �̃C���X�^���X�Ƃ̊֘A�Â����O�ꂽ���ɒʒm����܂��B
		 * @param source �֘A�t�����ꂽ�I�u�W�F�N�g
		 * @endja
		 */
		virtual void Unbound(B* source) = 0;
	};

	/**
	 * @ja
	 * VCE ��������̒ʒm�������Ƃ�C���^�[�t�F�C�X�B
	 *
	 * VCE �����̐ڑ��I�u�W�F�N�g���� Session �ɑ΂���Ăяo�����t�b�N���܂��B
	 * ���̃C���^�[�t�F�C�X�� VCE::BindStream() �Ɏw�肷�邱�ƂŁAVCE ���� Session �ւ̌Ăяo���������ł��܂��B
	 * ����肵���R�[���o�b�N�� Session ���󂯎�邱�Ƃ͂���܂���B
	 *
	 * @see VCE::BindStream()
	 * @endja
	 */
	class LoopReaction : public Reaction<LoopBinding>
	{
	public:
		virtual ~LoopReaction() {}

		/**
		 * @ja
		 * �ڑ��I�u�W�F�N�g�� IO �������s���ׂ��^�C�~���O���ʒm����܂��B
		 * Session �ɂ͒��ڊY������ API �͂���܂��񂪁A
		 * �ʏ�� Session �ł� IO �����̌��ʂƂ��� Session::Recv() �� Session::Closed() �Ȃ� 
		 * IO �֌W�̒ʒm���s���܂��B
		 * �]���āA���p�҂� Session �̃R�[���o�b�N�����z������ꍇ�A
		 * IO �֌W�̒ʒm�͂��� Polled() ������s���̂��K�؂ł��B
		 * @endja
		 */
		virtual void Polled(LoopBinding* source) = 0;

		/**
		 * @ja
		 * ���p�҂��K���ȊԊu�ŉ��炩�̏������s�����߂̒ʒm�ł��B
		 * Session::Think() �ɑ������܂��B
		 *
		 * @see Session::Think()
		 * @endja
		 */
		virtual void Engaging(LoopBinding* source) = 0;
	};

	class AnyLoopReaction  : virtual public Any,
							 public LoopReaction
	{
	public:
		virtual void Bound(LoopBinding* /*adaptor*/) { Acquire(); }
		virtual void Unbound(LoopBinding* /*source*/) { Release(); }
	};

	/**
	 * @ja
	 * �����ɂ���ڑ��I�u�W�F�N�g�̏�Ԃ�����킷�C���^�[�t�F�C�X�ł��B
	 * @endja
	 *
	 * @see StreamBinding::GetState()
	 */
	class StreamState : virtual public Any
	{
	public:
		/** @ja @return Session::GetState() �ɊY�������� */
		virtual VCE_STATE GetState() const = 0;
		/** @ja @return Session::GetKeyexState() �ɊY�������� */
		virtual Session::KEYEX_STATE GetKeyexState() const = 0;
	};

	/**
	 * @ja
	 * �O�������Ԃ��w��ł��� StreamState �̊g���C���^�[�t�F�C�X�ł��B
	 * ��ɗ��p�҂� StreamState ����������֋X���߂̂��߂ɗp�ӂ���Ă��܂��B
	 * MakeMutableStreamState() �ŃC���^���X�𐶐����邱�Ƃ��ł��܂��B
	 *
	 * @see MakeMutableStreamState()
	 * @endja
	 */
	class MutableStreamState : public StreamState
	{
	public:
		/**
		 * @ja
		 * @param s StreamState::GetState() �̖߂�l���w�肵�܂��B
		 * @endja
		 */
        virtual void SetState(vce::VCE_STATE s) = 0;

		/**
		 * @ja
		 * @param s StreamState::GetKeyexState() �̖߂�l���w�肵�܂��B
		 * @endja
		 */
		virtual void SetKeyexState(Session::KEYEX_STATE s) = 0;
	};

	/**
	 * @ja
	 * MutableStreamState �̃C���X�^���X�𐶐����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ��邽�߂̃������A���P�[�^
	 * @return �������ꂽ�C���X�^���X
	 * @endja
	 */
	MutableStreamState* MakeMutableStreamState(MemoryAllocator* ma);

	/**
	 * @ja
	 * �ڑ��I�u�W�F�N�g�� TPC/IP ���x���ł̑���������킷�C���^�[�t�F�C�X�ł��B
	 *
	 * @see StreamBinding::GetProperties()
	 * @endja
	 */
	class StreamProperties : virtual public Any
	{
	public:
		/** @ja @return Session::GetRemoteAddress() �� addr �ϐ��ɊY�� */
		virtual VUint32 GetRemoteAddress() const = 0;
		/** @ja @return Session::GetRemoteAddress() �� port �ϐ��ɊY�� */
		virtual VUint16 GetRemotePort() const = 0;
		/** @ja @return Session::GetRemoteHostname() �ɊY�� */
		virtual const char* GetRemoteHostname() const = 0;
		/** @ja @return Session::GetLocalAddress() �� addr �ϐ��ɊY�� */
		virtual VUint32 GetLocalAddress() const = 0;
		/** @ja @return Session::GetLocalAddress() �� port �ϐ��ɊY�� */
		virtual VUint16 GetLocalPort() const = 0;
	};


	class StreamReaction; // for cyclic ref on StreamBinding

	/**
	 * @ja
	 * �f�[�^�������s���o�����X�g���[������킷�C���^�[�t�F�C�X�ł��B
	 *
	 * ���p�҂��� Session �I�u�W�F�N�g�ւ� API �Ăяo�����t�b�N���܂��B
	 * ���̃C���^�[�t�F�C�X�� VCE::BindStream() �Ɏw�肷�邱�ƂŁA
	 * ���p�҂� Session �ɑ΂��čs���� API �Ăяo���������ł��܂��B
	 * ����肵�� API �� VCE ���󂯎�邱�Ƃ͂���܂���B
	 * 
	 * @see VCE::BindStream()
	 * @endja
	 */
	class StreamBinding : virtual public Any,
						  public Unreactable<StreamReaction>,
						  public Inspectable
	{
	public:
		virtual ~StreamBinding() {}

		/**
		 * @ja
		 * �X�g���[����ؒf���܂��B
		 * Session::Close() �ɑ������܂��B
		 * @endja
		 */
		virtual void Close() = 0;

		/** Close() �̕ʖ��ł� */
		void Unbind() { Close(); }

		/**
		 * @ja
		 * �X�g���[����ؒf���܂��B
		 * Session::ForceClose() �ɑ������܂��B
		 * @endja
		 */
		virtual void ForceClose() = 0;

		/** @ja @return ������Ԃ�����킷�I�u�W�F�N�g */
        virtual StreamState* GetState() const = 0;
		/** @ja @return �ڑ�����������킷�I�u�W�F�N�g */
        virtual StreamProperties* GetProperties()= 0;
		/** @ja @return ���M�o�b�t�@ */
		virtual StreamBuffer* GetSendBuffer() = 0;
		/** @ja @return ��M�o�b�t�@ */
		virtual StreamBuffer* GetRecvBuffer() = 0;
		/** @ja Session::SetNodelay() �ɊY�� */
		virtual void SetNodelay() = 0;
		/** @ja ���M�o�b�t�@���l�b�g���[�N�Ƀf�[�^���������݂܂� */
		virtual bool Send(const void* buf, size_t size);
		/** @ja �v���b�g�z�[���� IO ������킷�f�X�N���v�^�ł��B���݂��Ȃ��ꍇ�� -1 ��Ԃ��܂��B */
		virtual int GetDescriptor() const { return -1; } // return -1 for backword compatibility of subclasses.
		/** @ja Inspectable �̃f�t�H���g�����ł��B�������܂���B */
		virtual void InspectName(std::ostream& /*out*/) { /* */ }
	};

	/**
	 * @ja
	 * VCE ��������̒ʒm�������Ƃ�C���^�[�t�F�C�X�B
	 *
	 * Reaction ���l�A VCE �����̐ڑ��I�u�W�F�N�g���� Session �I�u�W�F�N�g�ɑ΂���ʒm�𒊏ۉ����Ă��܂��B
	 * ������ IO �����̃R�[���o�b�N���`���Ă���_�� Reaction �Ƃ͈ق�܂��B
	 * Reaction ����� Session �ւ̃R�[���o�b�N�����z�����邽�߂ɗp����̂ɑ΂��A
	 *
	 * StreamReaction �̓X�g���[���g���� Session ���ւ��邽�߂̃C���^�[�t�F�C�X�ł��B
	 * �p���w���� Session �ɑ΂��A StreamReaction �͗��p�҂ɃC���^�[�t�F�C�X�w���� API ��񋟂��܂��B
	 *
	 * VCE::ConnectAsStream() �� Multiplexer �𗘗p����ۂɒ�`����K�v������܂��B
	 * 
	 * @endja
	 */
	class StreamReaction : public Reaction<StreamBinding>
	{
	public:
		/**
		 * @ja
		 * Session::Recv() �ɊY�����܂��B
		 * @see Session::Recv()
		 * @endja
		 */
		virtual size_t Received(StreamBinding* source, const VUint8 *p,size_t sz) = 0;

		/**
		 * @ja
		 * Session::Connected() �ɊY�����܂��B
		 * @see Session::Connected()
		 * @endja
		 */
		virtual void Connected(StreamBinding* source) = 0;

		/**
		 * @ja
		 * Session::Closed() �ɊY�����܂��B
		 * @see Session::Closed()
		 * @endja
		 */
		virtual void Closed(StreamBinding* source, VCE_CLOSEREASON type) = 0;
		/**
		 * @ja
		 * Session::Exception() �ɊY�����܂��B
		 * @see Session::Exception()
		 * @endja
		 */
		virtual void Caught(StreamBinding* source, VCE_EXCEPTION type) = 0;

		/**
		 * @ja
		 * Session::Think() �ɊY�����܂��B
		 * @see Session::Exception()
		 * @endja
		 */
		virtual void Engaging(StreamBinding* source) = 0;
	};

	class AnyStreamReaction : virtual public Any,
							  public StreamReaction
	{
	public:
		virtual void Bound(StreamBinding* /*source*/) { Acquire(); }
		virtual void Unbound(StreamBinding* /*source*/) { Release(); }
	};

	/**
	 * �������Ȃ� StreamReaction �̃C���X�^���X��Ԃ��܂��B
	 * ��M�����o�C�g��͂����j������܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ��郁�����A���P�[�^
	 * @return StreamReaction NullStreamReaction �̃C���X�^���X
	 */
	StreamReaction* MakeNullStreamReaction(MemoryAllocator* /*ma*/);

	/**
	 * @ja 
	 * �N���C�A���g����̐ڑ��v���̓���������킷�C���^�[�t�F�C�X�ł��B
	 * ListenerReaction ����Q�Ƃ���܂��B
	 *
	 * StreamArrival �� Any ���p�����܂���B���������� StreamArrival �̎�����
	 * ListenerReaction::Arrived() �̌Ăяo�����̂ݗL���ŁA�������邱�Ƃ͂ł��܂���B
	 *
	 * @see ListenerReaction::Arrived()
	 * @endja
	 */
	class StreamArrival
	{
	public:
		/** @ja @return �ڑ����z�X�g�� IP �A�h���X */
		virtual VUint32 GetHost() const = 0;

		/** @ja @return �ڑ����z�X�g�̃|�[�g�ԍ� */
		virtual VUint32 GetPort() const = 0;

		/**
		 * @ja 
		 * �ڑ��v�����󂯕t���܂��B
		 *
		 * @param reaction �ڑ��̊m���ɉ����č����ڑ��X�g���[������������I�u�W�F�N�g�B
		 * @endja
		 */
		virtual void Accept(StreamReaction* reaction) = 0;
	};

	/**
	 * ListenerReaction ���牺���̎����ɃA�N�Z�X���邽�߂� API �ł��B
	 * ListenerReaction �Ɉ����n����܂��B
	 *
	 */
	class ListenerBinding : virtual public Any
	{
	public:
		friend class ListenerReaction;
		/**
		 * ListenerReaction �� VCE ����؂藣���܂��B
		 * �ʏ�̏I���V�[�P���X���J�n���A�ŏI�I�ɂ� Unbound() ���Ăяo����܂��B
		 */
		virtual void Unbind() = 0;

		/**
		 * ListenerReaction �𑦍��� VCE ����؂藣���܂��B Unbound() �͌Ăяo����܂���B
		 */
		virtual void Unreact(ListenerReaction* lisn) = 0;
		virtual VUint32 GetPort() const = 0;
	};

	/**
	 * @ja
	 * �ڑ��v���̑҂��󂯂��s���C���^�[�t�F�C�X�ł��B
	 * �X�g���[���g�� API �� Listener ���ւ��܂��B
	 * @endja
	 */
	class ListenerReaction : public Reaction<ListenerBinding>
	{
	public:
		/**
		 * @ja
		 * �ڑ��v���̓�����ʒm���܂��B
		 * 
		 * @param incoming ���������ڑ��v��
		 * @endja
		 */
		virtual void Arrived(ListenerBinding* source, StreamArrival* incoming) = 0;

		virtual void Caught(ListenerBinding* source, vce::VCE_EXCEPTION type) = 0;
	};

	class AnyListenerReaction : virtual public Any,
								public ListenerReaction
	{
	public:
		virtual void Bound(ListenerBinding* /*source*/) { Acquire(); }
		virtual void Unbound(ListenerBinding* /*source*/) { Release(); }
	};

	/* ------------------------------------------------------------
	 *
	 * �X�g���[���g���𗘗p����g���݃I�u�W�F�N�g
	 *
	 * ------------------------------------------------------------ */

	/**
	 * @ja
	 * �I���������œ��삷�� StreamBuffer �̎��� MemoryStreamBuffer �̃C���X�^���X��
	 * �V���ɐ������܂��B�Ǝ��� StreamBinding ����������ۂ̗��p��z�肵�Ă��܂��B
	 *
	 *
	 * @bufsize ����M�̊e�o�b�t�@�̏����T�C�Y���w������o�C�g��
	 * @ma �C���X�^���X���m�ۂ��郁�����A���P�[�^
	 * @endja
	 */
	StreamBuffer* MakeMemoryStreamBuffer(MemoryAllocator *ma, size_t bufsize);

	/**
	 * @ja
	 * �I���������œ��삷�� StreamBinding ������킷�g���C���^�[�t�F�C�X�ł��B
	 * MemoryStreamBinding �I�u�W�F�N�g�̓�������Ƀo�b�t�@�������Ă��܂��B
	 * ���̂��߃��[�U�͎�M�o�b�t�@�ɏ������񂾂�A���M�o�b�t�@��ǂݏo�����Ƃ��ł��܂��B
	 * 
	 * Session �Ɋ StreamBinding �͓��o�͂Ń\�P�b�g�𑀍삵�܂����A
	 * MemoryStreamBinding �̓f�[�^������̑��M�o�b�t�@�ɗ��ۂ��܂��B
	 * �܂��A�����̎�M�o�b�t�@�֎����I�Ƀf�[�^����������Ƃ͂Ȃ��A
	 * ���p�҂����M�o�b�t�@�̒��g�����z�I�ɍ�肾���K�v������܂��B
	 *
	 *
	 * �ʐM��p���Ȃ��P�̃e�X�g�ł̗��p�Ȃǂ��Ӑ}���Ă��܂��B
	 *
	 * @see MakeMemoryStreamBinding()
	 * @endja
	 */
	class MemoryStreamBinding : public StreamBinding
	{
	public:
		/**
		 * @ja
		 * �I�u�W�F�N�g���ؒf���ꂽ���ǂ�����Ԃ��܂��B
		 * StreamBinding::Close() ���Ă΂�� true �ɂȂ�A
		 * MemoryStreamBinding::ClearClosing() ���Ă΂��� false �ɖ߂�܂��B
		 *
		 * @return �N���[�Y����Ă���� true
		 * @endja
		 */
		virtual bool IsClosing() const = 0;

		/**
		 * @ja
		 * �I�u�W�F�N�g�̐ؒf��Ԃ����Z�b�g���܂��B
		 *
		 * @see MemoryStreamBinding::IsClosing()
		 * @see StreamBinding::Close()
		 * @endja
		 */
		virtual void ClearClosing() = 0;

		/**
		 * @ja
		 * �X�g���[���̏�Ԃ�����킷�I�u�W�F�N�g��Ԃ��܂��B
		 * StreamBinding::GetState() �ƈق�A���p�҂���Ԃ�ύX�ł��܂��B
		 *
		 * @return ��ԃI�u�W�F�N�g
		 * @see StreamBinding::GetState()
		 * @endja
		 */
		virtual MutableStreamState* GetMutableState() = 0;

		/**
		 * �o�^����Ă��� reaction ��Ԃ��܂��B
		 *
		 * @return StreamReaction
		 */
		virtual StreamReaction* GetReaction() = 0;
		
		/**
		 * �o�^����Ă��� reaction �� Unboud() ��ʒm���A�o�^���������܂��B
		 */
		virtual void Unbind() = 0;

		/** @param ins ���O�o�͂ɗ��p���� Inspectable ��ݒ肵�܂� */
		virtual void SetInspectable(Inspectable* ins) = 0;
		/** @return ���O�o�͂ɗ��p���� Inspectable */
		virtual Inspectable* GetInspectable() = 0;
	};

	/**
	 * MemoryStreamBinding �̃C���X�^���X�𐶐����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ��郁�����A���P�[�^
	 * @return ���������C���X�^���X
	 */
	MemoryStreamBinding* MakeMemoryStreamBinding(MemoryAllocator* ma, StreamReaction* reaction=0);

	/**
	 * @ja
	 *
	 * MemoryStreamBinding �̃C���X�^���X�𐶐����܂��B
	 * �o�b�t�@���Ԃ��O������^���邱�Ƃ��ł��܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ��郁�����A���P�[�^
	 * @param recv ��M�o�b�t�@
	 * @param send ���M�o�b�t�@
	 * @param state ��ԃI�u�W�F�N�g
	 * @param prop �����I�u�W�F�N�g
	 * @param reaction �֘A�Â����� reaction�B MemoryStreamReaction �Ȃ�
	 *                 �R�[���o�b�N�ʒm���s���I�u�W�F�N�g����Q�Ƃ���܂��B
	 * @return ���������C���X�^���X
	 *
	 * @endja
	 */
	MemoryStreamBinding* MakeMemoryStreamBinding(MemoryAllocator* ma, 
												 StreamBuffer* recv, StreamBuffer* send,
												 MutableStreamState* state, StreamProperties* prop,
												 StreamReaction* reaction=0);



	/**
	 * @ja
	 *
	 * BufferingStreamReaction �̃C���X�^���X�𐶐����܂��B
	 * BufferingStreamReaction �̓o�b�t�@�������Ȃ� StreamBinding �ɑ΂�
	 * �o�b�t�@��񋟂���A�^�v�^�ł��B
	 *
	 * @param ma �C���X�^���X���m�ۂ��郁�����A���P�[�^
	 * @param reaction �֘A�Â����� reaction
	 * @return ���������C���X�^���X
	 *
	 * @endja
	 */
	AnyStreamReaction* MakeBufferingStreamReaction(MemoryAllocator* ma, StreamReaction* reaction0);

	/**
	 * vce::Codec �̂悤�� Write() �ŏ������񂾍ۂ̃T�C�Y��ۑ����A
	 * ��M�ɂ���𕜌����Ēʒm���� StreamBinding �̎����ł��B
	 */
	class PacketStreamBinding : public StreamBinding
	{
	public:
		virtual AnyStreamReaction* GetSourceReaction() = 0; 
		virtual StreamReaction* GetTargetReaction() = 0;
		virtual StreamBinding* GetSourceBinding() = 0;
	};

	/**
	 * PacketStreamBinding �̃C���X�^���X�𐶐����܂��B
	 * @param ma �C���X�^���X���m�ۂ��郁�����A���P�[�^
	 * @return ���������C���X�^���X
	 */
	PacketStreamBinding* MakePacketStreamBinding(vce::MemoryAllocator* ma, StreamReaction* reaction);

	/**
	 * @ja
	 * MemoryStreamBinding �̏�Ԃ��A�w�肵�� StreamReaction �ɒʒm���邽�߂� Reaction �����ł��B
	 * MemoryStreamBinding �ƕ��p���邱�ƂŁASession ���I���������̃o�b�t�@��ɉ��z�����邱�Ƃ��ł��܂��B
	 *
	 * MemoryStreamBinder �́A MemoryStreamBinding �̏�ԂƁA
	 * ���z�I�Ȑڑ����̏�Ԃ𓯊�����悤�ɐU�����܂��B���Ƃ��ΐڑ�����Ԃ� VCE_STATE_CONNECTING
	 * �ɂȂ�� MemoryStreamBinding ���g�̏�Ԃ� VCE_STATE_CONNECTING �� VCE_STATE_ESTABLISHED �ɕω����A
	 * StreamReaction::Connected() �ȂǊY������ StreamReaction �̃R�[���o�b�N��ʒm���܂��B
	 * ���l�ɐڑ�����Ԃ� VCE_STATE_CLOSED �ƂȂ�Ύ��g�� VCE_STATE_CLOSED �ɑJ�ڂ��܂��B
	 *
	 * @see MakeMemoryStreamBinding()
	 * @endja
	 */
	class MemoryStreamBinder : virtual public Any
	{
	public:
		/**
		 * @ja
		 * ���z�������X�g���[����ŗ�O�𔭐������܂��B��O�͊֘A�Â���ꂽ StreamReaction �ɒʒm����܂��B
		 * ��O������A�X�g���[���͐ؒf���܂��B
		 * 
		 * @param ex �����������O�̎��
		 * @param cr ��O������ɍs���N���[�Y�̎��
		 * @endja
		 */
		virtual void NotifyRaised(VCE_EXCEPTION ex, VCE_CLOSEREASON cr) = 0;

		/**
		 * @ja
		 * ���z�������X�g���[����ؒf���܂��B �֘A�Â���ꂽ StreamReaction �ɒʒm���s���A
		 * �N���[�Y�̃V�[�P���X���J�n���܂��B
		 *
		 * @param cr �N���[�Y�̎��
		 * @endja
		 */
		virtual void NotifyClosedWith(VCE_CLOSEREASON cr) = 0;

		/** @ja �Ώۂ� MemoryStreamBinding */
		virtual MemoryStreamBinding* GetBinding() const = 0;

		/** @ja �o�^�ɗp���� LoopReaction */
		virtual LoopReaction* GetBound() = 0;
	};

	/**
	 * @ja
	 * MemoryStreamBinder �̃C���X�^���X�𐶐����܂��B
	 * 
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param binding0 �C���X�^���X���Ď����� MemoryStreamBinding
	 * @param reaction0 �C���X�^���X���ʒm���s�� StreamReaction
	 * @param remote_state ���z�I�ȒʐM���̏��
	 * @return ���������C���X�^���X
	 * @endja
	 */
	MemoryStreamBinder* 
	MakeMemoryStreamBinder(MemoryAllocator* ma, MemoryStreamBinding* binding0, StreamState*remote_state);


	/**
	 * @ja
	 * �������Ȃ� Reaction �I�u�W�F�N�g�𐶐����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @return ���������C���X�^���X
	 * @endja
	 */
	LoopReaction* MakeNullReaction(MemoryAllocator* ma);

	/**
	 * @ja
	 * VCE ���g�����f�[�^�������ASession ����ăX�g���[���t���[�����[�N�� API �ŗ��p����ϊ��I�u�W�F�N�g�ł��B
	 * MakeSessionStreamBinder() �o�R�ō\�z���� Session �� VCE::Connect() �Ȃǂœo�^����ƁA
	 * �o�^���� Session ����� �w��� StreamReaction �ɃR�[���o�b�N���ʒm����܂��B
	 * �܂� StreamReaction �Ɋ֘A�Â���ꂽ StreamBinding �ɑ΂��� IO ������s���ƁA
	 * ������ IO �� Session ����� VCE �Ɉڏ�����܂��B
	 * �܂�A StreamBinding �� StreamReaction �� Session �Ń��b�v���܂��B
	 *
	 * �]���ǂ��� Session �𗘗p���ăv���O���~���O������ꍇ�A SessionStreamBinder �𗘗p����K�v�͂���܂���B
	 * �X�g���[���g���� API ��ʂ��� VCE �𗘗p���邽�߂ɗp�ӂ���Ă��܂��B
	 *
	 * @endja
	 */
	class SessionStreamBinder : virtual public Any
	{
	public:
		/** @ja @return VCE �ɓo�^���邽�߂� Session */
		virtual Session* GetBound() = 0;
		/** @ja @return Session �ɑ΂���X�g���[������ */
		virtual StreamBinding* GetBinding() = 0;
	};

	/**
	 * @ja
	 * SesesionStreamBinder �̃C���X�^���X�𐶐����܂��B
	 * �����Ő�������� SessionStreamBinder �� Session �𗘗p���Ă���,
	 * Session::Recv() �� Session::Send() �� �o�C�g��̓��o�͂ɗ��p����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param reaction Session �Ɋ֘A�Â��郊�A�N�V�����I�u�W�F�N�g
	 * @param llflags Session �ɐݒ肷��Í���/�����������Ȃǂ̃t���O�BSession::llflags �ɊY���B
	 * @return �������ꂽ�C���X�^���X
	 * @see Session::llflags
	 * @endja
	 */
	SessionStreamBinder* MakeSessionStreamBinder(MemoryAllocator* ma, StreamReaction* reaction,
												 unsigned int llflags=Session::LLF_NONE);

	/**
	 * @ja
	 * SesesionStreamBinder �̃C���X�^���X�𐶐����܂��B
	 * �����Ő�������� SessionStreamBinder �� Codec �𗘗p���Ă���,
	 * Codec::Parsed() �� Codec::Merge() �� �o�C�g��̓��o�͂ɗ��p����܂��B
	 *
	 * ��������: 
	 * ���� SessionStreamBinder �ɗ^������ StreamReaction �� StreamReaction::Received() ��
	 * �Ăяo���ꂽ�ہA�S�Ẵf�[�^����������K�v������܂��B����� Codec �̎����ɋN�����鐧���ł��B
	 * �܂����l�̐����ɂ��AGetBinding() �Ŏ擾�ł��� StreamBinding �́A
	 * StreamBuffer::Peek() �Ńo�b�t�@���Q�Ƃ�����, 
	 * StreamBuffer::Shrink() �Ńo�b�t�@���폜���邱�Ƃ͂ł��܂���B
	 *
	 * Codec �Ɋ֘A�Â���ꂽ SessionStreamBinder �́A�T�C�Y�t���o�C�g���p���� RPC �ʐM��
	 * ���p����邱�Ƃ�z�肵�Ă��܂��B
	 * ���_��ȃf�[�^�Ǘ��������������ꍇ�� MakeSessionStreamBinder() �𗘗p���Ă��������B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param reaction Session �Ɋ֘A�Â��郊�A�N�V�����I�u�W�F�N�g
	 * @param llflags Session �ɐݒ肷��Í���/�����������Ȃǂ̃t���O�BSession::llflags �ɊY���B
	 * @return �������ꂽ�C���X�^���X
	 * @see Session::llflags
	 * @endja
	 */
	SessionStreamBinder* MakeCodecStreamBinder(MemoryAllocator* ma, StreamReaction* reaction,
											   unsigned int llflags=0);

	/**
	 * @ja
	 * VCE ���� Listener �ւ̐ڑ��v���� ListenerReaction �ŉ����邽�߂̕ϊ��I�u�W�F�N�g�ł��B
	 * MakeSessionListenerBinder() �o�R�ō\�z���� Listener �� VCE::Listen() �Ȃǂœo�^����ƁA
	 * �o�^���� Listener ����� �w��� ListenerReaction �ɃR�[���o�b�N���ʒm����܂��B
	 *
	 * SessionStreamBinder ���l�A�X�g���[���g���� API ��ʂ��� VCE �𗘗p���邽�߂ɗp�ӂ���Ă��܂��B
	 * @endja
	 */
	class ListenerBinder : virtual public Any
	{
	public:
		/** @ja @return VCE �ɓo�^���邽�߂� Listener */
		virtual Listener* GetBound() = 0;
	};

	/**
	 * @ja
	 * Session ���쐬���� ListenerBinder �̃C���X�^���X�𐶐����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param lisn Listener �Ɋ֘A�Â��� ListenerReaction �I�u�W�F�N�g
	 * @param llflags �I�u�W�F�N�g���ڑ���t�ɍ\�z���� Session �Ɏw�肷�� llflags
	 * @return �������ꂽ�C���X�^���X
	 *
	 * @see Session::llflags
	 *
	 * @endja
	 */
	ListenerBinder* MakeSessionListenerBinder(MemoryAllocator* ma, ListenerReaction* lisn, 
											  unsigned int llflags=Session::LLF_NONE);

	/**
	 * @ja
	 * Codec ���쐬���� ListenerBinder �̃C���X�^���X�𐶐����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param lisn Listener �Ɋ֘A�Â��� ListenerReaction �I�u�W�F�N�g
	 * @param llflags �I�u�W�F�N�g���ڑ���t�ɍ\�z���� Session �Ɏw�肷�� llflags
	 * @return �������ꂽ�C���X�^���X
	 *
	 * @see Session::llflags
	 *
	 * @endja
	 */
	ListenerBinder* MakeCodecListenerBinder(MemoryAllocator* ma, ListenerReaction* lisn, 
											unsigned int llflags=Session::LLF_NONE);

	/**
	 * @ja
	 * �w�肵�� Session �Ɍ��т��� StreamBinding �̃C���X�^���X�𐶐����܂��B
	 * �������� StreamBinding �ւ� API �Ăяo���́A tobound �Ɏw�肵�� Session �Ɉڏ�����܂��B
	 * SessionStreamBinder �Ƃ͋t�ɁASession �� StreamBinding �Ń��b�v���邱�Ƃ��ł��܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param tobound API �̈ڏ��� Session
	 * @return �������ꂽ�C���X�^���X
	 * @endja
	 */
	StreamBinding* MakeSessionStreamBinding(MemoryAllocator* ma, Session* tobound, Unreactable<StreamReaction>* unr);

	/**
	 * @ja
	 * �w�肵�� Session �Ɍ��т��� StreamReaction �̃C���X�^���X�𐶐����܂��B
	 * �������� StreamReaction �ւ� API �Ăяo���́A tobound �Ɏw�肵�� Session �Ɉڏ�����܂��B
	 * SessionStreamBinder �Ƃ͋t�ɁASession �� StreamReaction �Ń��b�v���邱�Ƃ��ł��܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param tobound API �̈ڏ��� Session
	 * @return �������ꂽ�C���X�^���X
	 * @endja
	 */
	AnyStreamReaction* MakeSessionStreamReaction(MemoryAllocator* ma, Session* target);

	/**
	 * @ja
	 * �w�肵�� Listener �Ɍ��т��� ListenerReaction �̃C���X�^���X�𐶐����܂��B
	 * �������� StreamReaction �ւ� API �Ăяo���́A tobound �Ɏw�肵�� Listener �Ɉڏ�����܂��B
	 * ListenerBinder �Ƃ͋t�ɁAListener �� ListenerReaction �Ń��b�v���邱�Ƃ��ł��܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param tobound API �̈ڏ��� Listener
	 * @return �������ꂽ�C���X�^���X
	 *
	 * @endja
	 */
	AnyListenerReaction* MakeSessionListenerReaction(VCE* api, Listener* tobound);

	/**
	 * @ja
	 * �w�肵�� Session �Ɍ��т��� StreamReaction �̃C���X�^���X�𐶐����܂��B
	 * MakeSessionStreamReaction() �Ƃ悭���Ă��܂����A 
	 * MakeSessionReaction() �Ő�������̂� Reaction �I�u�W�F�N�g�ł��B
	 * ���������� IO �Ɋ֌W���� API �͈ڏ����ꂸ�A Engaging() �Ȃǌ���ꂽ API �������ڏ�����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param tobound API �̈ڏ��� Session
	 * @return �������ꂽ�C���X�^���X
	 * @endja
	 */
	LoopReaction* MakeSessionReaction(MemoryAllocator* ma, Session* target);

	/**
	 * @ja
	 * ��� Session lead �� peer ��Z�����邽�߂̃I�u�W�F�N�g�ł��B
	 * �Z�����ꂽ����� Sesion ����̑��M�́A��������Ɏ�M�Ƃ��Ēʒm����܂��B
	 * ���l�� Close() ����������������֓`�d���܂��B
	 *
	 * �Z���́A�ȈՌ`���̃��[�v�o�b�N�@�\�ł��B�C���v���Z�X�T�[�o���������邽�߂́A
	 * ��֎�i��񋟂��܂��B���[�v�o�b�N�Ɣ�ׁA�d�g�݂��Ǐ��I�Ƃ������_������܂��B
	 * �{���� Session �̓���Ƃ̌݊����̓��[�v�o�b�N��艺����܂��B
	 *
	 * @endja
	 */
	class SessionShortcut : virtual public Any
	{
	public:
		/**
		 * @ja
		 * ��� Session �̊ԂŐڑ����m�����܂��B
		 * @endja
		 */
		virtual void Connect() = 0;

		/**
		 * lead �� Session �ŗ�O�𔭐����܂��B��O������A�X�g���[���͐ؒf����܂��B
		 * @param ex �����������O�̎��
		 * @param local_cr lead �ɒʒm�����ؒf���
		 * @param remote_cr peer �ɒʒm�����ؒf���
		 */
		virtual void RaiseToLead(VCE_EXCEPTION ex, VCE_CLOSEREASON local_cr, VCE_CLOSEREASON remote_cr) = 0;

		/**
		 * peer �� Session �ŗ�O�𔭐����܂��B��O������A�X�g���[���͐ؒf����܂��B
		 * @param ex �����������O�̎��
		 * @param local_cr peer �ɒʒm�����ؒf���
		 * @param remote_cr lead �ɒʒm�����ؒf���
		 */
		virtual void RaiseToPeer(VCE_EXCEPTION ex, VCE_CLOSEREASON local_cr, VCE_CLOSEREASON remote_cr) = 0;

		/** @ja @return lead ���� Session */
		virtual Session* GetLead() = 0;
		/** @ja @return peer ���� Session */
		virtual Session* GetPeer() = 0;
		/** @ja @return lead ���� Session �Ɍ��т��� StreamBinding */
		virtual StreamBinding* GetLeadBinding() = 0;
		/** @ja @return peer ���� Session �Ɍ��т��� StreamBinding */
		virtual StreamBinding* GetPeerBinding() = 0;
	};
	
	/**
	 * @ja
	 * SessionShortcut �̃C���X�^���X�𐶐����܂�
	 * 
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @param lead lead ���ŒZ������ Session
	 * @param peer peer ���ŒZ������ Session
	 * @return �������ꂽ�C���X�^���X
	 *
	 * @endja
	 */
	SessionShortcut* ShortcutSessions(VCE* api, Session* lead, Session* peer);

	/**
	 * @ja
	 * ���[�J���ƃ����[�g�̃|�[�g�΂ł��B
	 * Multiplexer ��ł̐ڑ������ʂ��邽�߂Ɏg���܂��B
	 * @endja
	 */
	struct PortPair
	{
		explicit PortPair(VUint32 local0=0, VUint32 remote0=0) : local(local0), remote(remote0) {}
		/** @todo */
		VUint32 local;
		/** @todo */
		VUint32 remote;
	};

	/**
	 * @ja
	 * PortPair �� std::map �� std::set �̃L�[�Ƃ��ēo�^���邽�߂̔�r���Z�q�B
	 * @endja
	 */
	inline bool operator<(const PortPair& x, const PortPair& y)
	{
		if (x.local < y.local) {
			return true;
		} else if (x.local > y.local) {
			return false;
		} else if (x.remote < y.remote) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * @ja
	 * Multiplexer �����Ŕ��������O�B MultiplexerReaction �ɒʒm����܂��B
	 * ��O�̎�ʂɂ���Ă̓T�u�N���X����`����A�ڍׂȏ����Ƃ肾�����Ƃ��ł��܂��B
	 *
	 * @see Multiplexer
	 * @see MultiplexerReaction::Caught()
	 * @endja
	 */
	class MuxException
	{
	public:
		/** 
		 * @ja
		 * ��O���
		 *
		 * @see MuxException::GetType()
		 * @endja
		 */
		enum Type
		{
			/**
			 * @ja 
			 * �v�����ꂽ�|�[�g�ɉ����o�^����Ă��Ȃ�
			 * @endja
			 */
			TYPE_MISSING_PORT,
			/**
			 * @ja
			 * ���z�o�H��̃f�[�^�̏������s�� 
			 * @endja
			 */
			TYPE_WRONG_MESSAGE,
			/**
			 * @ja
			 * VCE �����ł̕s���������o�B�������ׂ��łȂ��B
			 * @endja
			 */
			TYPE_INTERNAL,
			/**
			 * @ja 
			 * �z�X�g�o�H��� VCE �̗�O�������B
			 * @see MuxVCEException
			 * @endja
			 */
			TYPE_VCE_EXCEPTION,
			/** @internal */
			TYPES
		};

		/** @internal */
		explicit MuxException(Type type0, const PortPair& where0=PortPair(0, 0)) 
			: type(type0), where(where0)
		{}

		/** @ja @return ��O��� */
		Type GetType() const { return this->type; }
		/** @ja @return ��O�̔����������z�o�H�B����ł��Ȃ��ꍇ�� (0,0) */
		const PortPair& GetWhere() const { return this->where; }
	private:
		Type type;
		PortPair where;
	};

	/**
	 * @ja
	 * MuxException::TYPE_VCE_EXCEPTION �ɑΉ����� MuxException �̃T�u�N���X�ł��B
	 * @endja
	 */
	class MuxVCEException : public MuxException
	{
	public:
		/** @internal */
		MuxVCEException(VCE_EXCEPTION raised0)
			: MuxException(TYPE_VCE_EXCEPTION, PortPair(0, 0)), raised(raised0)
		{}

		/** @ja @return �������� VCE ��O�̎�� */
		VCE_EXCEPTION GetRaised() const { return this->raised; }
	private:
		VCE_EXCEPTION raised;
	};

	class MultiplexerReaction; // for cyclic ref by Unreactable.

	/**
	 * @ja
	 * ��̃X�g���[����ŕ����̃X�g���[�����f�[�^���������邽�߂�
	 * MakeMultiplexer() �� reaction  �����Ɏw�肵���z�X�g�X�g���[���𑽏d�����܂��B
	 *
	 * �X�g���[���𑽏d������I�u�W�F�N�g�ł��B���d�������X�̃X�g���[�����T�u�X�g���[���ƌĂт܂��B
	 * @endja
	 */
	class Multiplexer : virtual public Any,
						public Unreactable<MultiplexerReaction>,
						public Inspectable
	{
	public:
		/**
		 * @ja
		 * ���d�������o�H��ł̐ڑ���v�����܂��B
		 * 
		 * @param port �ڑ���̃|�[�g�ԍ��B�ڑ����̃|�[�g�ԍ��͎����I�Ɋ���U���܂��B
		 * @param reaction �ڑ������o�H������ StreamReaction
		 * @endja
		 */
		virtual bool ConnectTo(VUint32 port, StreamReaction* reaction) = 0;

		/**
		 * @ja
		 * ���d�������o�H��ł̐ڑ��v����҂��󂯂܂��B
		 * 
		 * @param port �҂��󂯂郍�[�J���̃|�[�g�ԍ�
		 * @param listener �Y���|�[�g�ւ̐ڑ��v�������� ListenerReaction
		 * @endja
		 */
		virtual void ListenAt(VUint32 port, ListenerReaction* listener) = 0;

		/**
		 * @ja
		 * Multiplexer::ListenAt() �œo�^�����ڑ��҂����������܂��B
		 * ���ɓo�^�����|�[�g�ԍ����w�肷��K�v������܂��B
		 *
		 * @param port �ڑ��҂����������郍�[�J���̃|�[�g�ԍ�
		 * @endja
		 */
		virtual void UnlistenAt(VUint32 port) = 0;

		/**
		 * @ja
		 * ���d�������o�H��ł̐ڑ��v����҂��󂯂܂��B
		 * Listen() �œo�^���� ListenerReaction ��
		 * ListenAt() �œo�^�����҂��󂯂ƈ�v���Ȃ������ꍇ�ɗ��p����܂��B
		 *
		 * @endja
		 */
		virtual void Listen(ListenerReaction* listener) = 0;

		/**
		 * @ja
		 * Multiplexer::ListenAt() �œo�^�����S�Ă̐ڑ��҂����������܂��B
		 *
		 * @endja
		 */
		virtual void Unlisten() = 0;

		/** @internal */
		virtual AnyStreamReaction* GetTrunkReaction() = 0;

		/** @ja @return ���d�������o�H��Őڑ��̊m�������T�u�X�g���[���̐� */
		virtual size_t GetSubstreamSize() const = 0;
		/** @ja @return ���d�������o�H��̃T�u�X�g���[���ڑ��ɑ΂��҂��󂯂����Ă��郊�X�i�[�̐� */
		virtual size_t GetSublistenerSize() const = 0;

		/**
		 * @ja
		 * �z�X�g�X�g���[����ؒf���܂��B
		 * @endja
		 */
		virtual void Close() = 0;

		/** Close() �̕ʖ��ł� */
		void Unbind() { Close(); }

		/**
		 * @ja
		 * �z�X�g�X�g���[����ؒf���܂��B
		 * @endja
		 */
		virtual void ForceClose() = 0;

		/** @ja @return �z�X�g�X�g���[���̓�����Ԃ�����킷�I�u�W�F�N�g */
        virtual StreamState* GetState() const = 0;
		/** @ja @return �z�X�g�X�g���[���̐ڑ�����������킷�I�u�W�F�N�g */
        virtual StreamProperties* GetProperties()= 0;
		/** @ja �v���b�g�z�[���� IO ������킷�f�X�N���v�^�ł��B���݂��Ȃ��ꍇ�� -1 ��Ԃ��܂��B */
		virtual int GetDescriptor() const { return -1; } // has default to keep  backword compatibility
	};

	/**
	 * @ja
	 * Multiplexer ����A���d�������X�g���[���Ɉڏ�����Ȃ��ʒm�������Ƃ邽�߂̃C���^�[�t�F�C�X�ł��B
	 * Multiplexer �̓z�X�g�X�g���[������̒ʒm�����߂��A�K�؂ȃT�u�X�g���[���ɔz�M���܂��B
	 * MultiplexerReaction �̓T�u�X�g���[���ɔz�M����̂��K�؂łȂ��ʒm���󂯎��܂��B
	 * @endja
	 */
	class MultiplexerReaction : public Reaction<Multiplexer>
	{
	public:
		/**
		 * @ja
		 * �z�X�g�X�g���[���̐ڑ����m���������Ƃ�ʒm���܂��B
		 * SessionReaction::Connected() �ɊY�����܂��B
		 * @endja
		 */
		virtual void Connected(Multiplexer* source) = 0;

		/**
		 * @ja
		 * �z�X�g�X�g���[�����ؒf���ꂽ���Ƃ�ʒm���܂��B
		 * �ؒf�ʒm�͊e�T�u�X�g���[���ɔz�M���ꂽ���ƁA���̃R�[���o�b�N�ɒʒm����܂��B
		 * SessionReaction::Closed() �ɊY�����܂��B
		 *
		 * @param type �ؒf���
		 * @endja
		 */
		virtual void Closed(Multiplexer* source, VCE_CLOSEREASON type) = 0;

		/**
		 * �z�X�g�X�g���[���Ŕ���������O��ʒm���܂��B
		 *
		 * @param ex ����������O
		 */
		virtual void Caught(Multiplexer* source, const MuxException& ex) = 0;

		/**
		 * @ja
		 * VCE �� Poll ���[�v�������I�ɒʒm����܂��B
		 * Session::Think() �ɊY�����܂��B
		 * @endja
		 */
		virtual void Engaging(Multiplexer* source) = 0;
	};

	class AnyMultiplexerReaction : virtual public Any,
								   public MultiplexerReaction
	{
	public:
		virtual void Bound(Multiplexer* /*source*/) { Acquire(); }
		virtual void Unbound(Multiplexer* /*source*/) { Release(); }
	};

	/**
	 * @ja
	 * �������Ȃ� MultiplexerReaction �I�u�W�F�N�g�𐶐����܂��B
	 *
	 * @param ma �C���X�^���X���m�ۂ���A���P�[�^
	 * @return ���������C���X�^���X
	 * @endja
	 */
	MultiplexerReaction* MakeNullMultiplexerReaction(MemoryAllocator* /*ma*/);

	/**
	 * @ja
	 * Multiplexer �̃C���X�^���X�𐶐�����
	 * 
	 * @param api Multiplexer �������ŗ��p���� Session ��o�^���� VCE �I�u�W�F�N�g
	 * @param reaction ���d������X�g���[���Ɋ֘A�Â���ꂽ StreamReaction
	 * @return ���������C���X�^���X
	 * @endja
	 */
	Multiplexer* MakeMultiplexer(VCE* api, MultiplexerReaction* reaction);


	/**
	 * @ja
	 * Any �̎Q�ƃJ�E���g�v���g�R�����������邽�߂̕⏕�I�u�W�F�N�g�B
	 * Any ���v�z����N���X���p�����ėp���邱�Ƃ��ł��܂��B
	 *
	 * @see Any
	 * @endja
	 */
	class CountedHelper
	{
	public:
		CountedHelper() : count(0) {}
		virtual ~CountedHelper() {}

		/**
		 * @ja
		 * �Q�ƃJ�E���g�𑝂₵�܂��B ���p����N���X�� Acquire() ����Ăяo���Ƒz�肵�Ă��܂��B
		 *
		 * @see Any::Acquire()
		 * @endja
		 */
		void DoAcquire() { count++; }

		/**
		 * @ja
		 * �Q�ƃJ�E���g�����炵�܂��B���p����N���X�� Release() ����Ăяo���Ƒz�肵�Ă��܂��B
		 *
		 * @see Any::Release()
		 * @endja
		 */
		void DoRelease() { if (0 == --count) { delete this; } }

		/**
		 * @ja
		 * �Q�ƃJ�E���g�����炵�܂��B�����Ȃ��� DoRelease() �ƈق�A
		 * ���̊֐��ł͎Q�ƃJ�E���g���[���ɂȂ����I�u�W�F�N�g�̉����
		 * ���� a �Ŏw�肵���A���P�[�^��p���܂��B
		 *
		 * ���p�҂͉������C���X�^���X�̃|�C���^ ptr �����N���X�Ƃ��ēn���K�v������܂��B
		 * ���Ԃ����e�N���X�̌^�̂Ƃ��ă|�C���^��n���ƁA�������m�ێ��ƃA�h���X���قȂ�G���[�ƂȂ�܂��B
		 *
		 * @param a �C���X�^���X����ɗp����A���P�[�^
		 * @param ptr �������I�u�W�F�N�g�B
		 * @endja
		 */
		template<class T>
		void DoRelease(MemoryAllocator* a, T* ptr) { if (0 == --count) { ptr->~T(); a->free(ptr); } }
	private:
		CountedHelper(const CountedHelper&);
		const CountedHelper& operator=(const CountedHelper&);
	private:
		int count;
	};

	/**
	 * StreamBinding �����т��� Session �T�u�N���X������킷�񋓎q�ł��B
	 * @see StreamConnectTCP(), StreamListenTCP()
	 */
	enum StreamSessionTypes
	{
		/** vce::Session */
		STREAM_SESSION_SESSION,
		/** vce::Codec */
		STREAM_SESSION_CODEC,
		/* �ԕ� */
		STREAM_SESSION_TYPES
	};

	/**
	 * @ja
	 *
	 * Session �� Codec ����� StreamReaction �� TCP �\�P�b�g�ɐڑ����܂��B
	 * VCE::Connect() �ւ̃V���[�g�J�b�g�ł��B
	 *
	 * @param Session ��o�^���� VCE �I�u�W�F�N�g
	 * @param Session �Ɋ֘A�Â���R�[���o�b�N
	 * @param hostname �ڑ���z�X�g��
	 * @param port �ڑ���|�[�g
	 * @param timeout �ڑ��̃^�C���A�E�g
	 * @param type ���p���� Session �̎��
	 * @param llflags Session �ɐݒ肷��Í����Ȃǂ̃t���O
	 * @return �o�^�Ɏg���� Session ��ێ����� binder
	 *
	 * @see VCE::Connect()
	 * @see Sesssion::llflags
	 *
	 * @endja
	 */
	SessionStreamBinder*
	StreamConnectTCP(VCE* api, StreamReaction *reaction, 
					 const std::string& hostname, VUint16 port,unsigned int timeout,
					 StreamSessionTypes type, unsigned int llflags=vce::Session::LLF_NONE);

	/**
	 * @ja
	 *
	 * Listener ����� ListenerReaction �ւ� TCP �ڑ��󂯕t���܂��B
	 * VCE::Listen() �ւ̃V���[�g�J�b�g�ł��B
	 * 
	 * @param api Listener ��o�^���� VCE �I�u�W�F�N�g
	 * @param listener Listener �ɓo�^���� ListenerReaction
	 * @param port �ڑ��҂��|�[�g
	 * @param bindaddress �ڑ��҂��A�h���X
	 * @param type ���p���� Session �̎��
	 * @param llflags Session �ɐݒ肷��Í����Ȃǂ̃t���O
	 * @return �o�^�Ɏg���� Listener ��ێ����� binder
	 *
	 * @see VCE::Listen()
	 * @endja
	 */
	ListenerBinder*
	StreamListenTCP(VCE* api, ListenerReaction* listener, 
					VUint16 port,const std::string& bindaddress,
					StreamSessionTypes type, unsigned int llflags=vce::Session::LLF_NONE);


	/*
	 * dummy Reaction implementation that does nothing.
	 */
	template<class B>
	class NullReaction : public Reaction<B>
	{
	public:
		static NullReaction<B>* GetInstance()
		{
			static NullReaction<B> s_instance;
			return &s_instance;
		}

		virtual void Bound(B* source) {}
		virtual void Unbound(B* source) {}
	};

	/*
	 * copied from boost/intrusiveptr.hpp
	 * Distributed under the Boost Software License, Version 1.0. (See
	 * accompanying file LICENSE_1_0.txt or copy at
	 * http://www.boost.org/LICENSE_1_0.txt)
	 */
	template<class T> class IntrusivePtr
	{
	private:

		typedef IntrusivePtr this_type;

	public:

		typedef T element_type;

		IntrusivePtr(): p_(0)
		{
		}

		IntrusivePtr(T * p, bool add_ref = true): p_(p)
		{
			if(p_ != 0 && add_ref) IntrusivePtrAddRef(p_);
		}

		template<class U> IntrusivePtr(IntrusivePtr<U> const & rhs): p_(rhs.Get())
		{
			if(p_ != 0) IntrusivePtrAddRef(p_);
		}

		IntrusivePtr(IntrusivePtr const & rhs): p_(rhs.p_)
		{
			if(p_ != 0) IntrusivePtrAddRef(p_);
		}

		~IntrusivePtr()
		{
			if(p_ != 0) IntrusivePtrRelease(p_);
		}

		template<class U> IntrusivePtr & operator=(IntrusivePtr<U> const & rhs)
		{
			this_type(rhs).swap(*this);
			return *this;
		}

		IntrusivePtr & operator=(IntrusivePtr const & rhs)
		{
			this_type(rhs).swap(*this);
			return *this;
		}

		IntrusivePtr & operator=(T * rhs)
		{
			this_type(rhs).swap(*this);
			return *this;
		}

		T * Get() const
		{
			return p_;
		}

		T & operator*() const
		{
			return *p_;
		}

		T * operator->() const
		{
			return p_;
		}

		operator bool () const
		{
			return p_ != 0;
		}

		typedef T * (this_type::*unspecified_bool_type)() const;
    
		operator unspecified_bool_type() const // never throws
		{
			return p_ == 0? 0: &this_type::Get;
		}

		bool operator! () const
		{
			return p_ == 0;
		}

		void swap(IntrusivePtr & rhs)
		{
			T * tmp = p_;
			p_ = rhs.p_;
			rhs.p_ = tmp;
		}

		this_type Clear()
		{
			this_type ret(0);
			ret.swap(*this);
			return ret;
		}

		void Unbound()
		{
			if (p_)
			{
				IntrusiveUnbound(Clear().p_);
			}
		}

		template<class S>
		void Unbound(S* source)
		{
			if (p_)
			{
				IntrusiveUnbound(Clear().p_, source);
			}
		}

		void Unreact()
		{
			if (p_)
			{
				IntrusiveUnreact(Clear().p_);
			}
		}

		template<class S>
		void Unreact(S* source)
		{
			if (p_)
			{
				IntrusiveUnreact(Clear().p_, source);
			}
		}

	private:

		T * p_;
	};

	template<class T, class U> inline bool operator<(IntrusivePtr<T> const & a, IntrusivePtr<U> const & b)
	{
		return a.Get() < b.Get();
	}

	template<class T, class U> inline bool operator==(IntrusivePtr<T> const & a, IntrusivePtr<U> const & b)
	{
		return a.Get() == b.Get();
	}

	template<class T, class U> inline bool operator!=(IntrusivePtr<T> const & a, IntrusivePtr<U> const & b)
	{
		return a.Get() != b.Get();
	}

	template<class T, class U> inline bool operator==(IntrusivePtr<T> const & a, U * b)
	{
		return a.Get() == b;
	}

	template<class T, class U> inline bool operator!=(IntrusivePtr<T> const & a, U * b)
	{
		return a.Get() != b;
	}

	template<class T, class U> inline bool operator==(T * a, IntrusivePtr<U> const & b)
	{
		return a == b.Get();
	}

	inline void IntrusivePtrAddRef(Any* ptr) { ptr->Acquire(); }
	inline void IntrusivePtrRelease(Any* ptr) { ptr->Release(); }

	template<class T>
	inline void IntrusiveUnbound(T* ptr) { ptr->Unbound(); }
	template<class T, class S>
	inline void IntrusiveUnbound(T* ptr, S* src) { ptr->Unbound(src); }
	template<class T>
	inline void IntrusiveUnreact(T* ptr) { ptr->Unreact(); }
	template<class T, class S>
	inline void IntrusiveUnreact(T* ptr, S* src) { ptr->Unreact(src); }

	template<class S, class Arg>
	inline void UnbindReaction(S** ptr, Arg* arg)
	{
		if (*ptr) {
			S* p = *ptr;
			*ptr = 0;
			p->Unbound(arg);
		}
	}

	template<class S, class Arg>
	inline void BindReaction(S** ptr, S* fresh, Arg* arg)
	{
		UnbindReaction(ptr, arg);
		*ptr = fresh;
		if (fresh)
		{
			fresh->Bound(arg);
		}
	}

	template<class S>
	inline void FlashReaction(S** ptr)
	{
		if (*ptr)
		{
			S* p = *ptr;
			*ptr = 0;
			p->Bound(0);
			p->Unbound(0);
		}
	}

	template<class BPtr>
	inline void UnbindBinding(BPtr* binding)
	{
		if (*binding) {
			(*binding)->Unbind();
			*binding = 0;
		}
	}

	template<class BPtr, class RPtr>
	inline void UnreactBinding(BPtr* binding, RPtr reaction)
	{
		if (*binding) {
			(*binding)->Unreact(reaction);
			*binding = 0;
		}
	}

	/*
	 * typedefs for ptr
	 */
	typedef IntrusivePtr<StreamBuffer> StreamBufferPtr;
	typedef IntrusivePtr<LoopBinding> LoopBindingPtr;
	typedef IntrusivePtr<StreamState> StreamStatePtr;
	typedef IntrusivePtr<MutableStreamState> MutableStreamStatePtr;
	typedef IntrusivePtr<StreamProperties> StreamPropertiesPtr;
	typedef IntrusivePtr<StreamBinding> StreamBindingPtr;
	typedef IntrusivePtr<PacketStreamBinding> PacketStreamBindingPtr;
	typedef IntrusivePtr<MemoryStreamBinding> MemoryStreamBindingPtr;
	typedef IntrusivePtr<MemoryStreamBinder> MemoryStreamBinderPtr;
	typedef IntrusivePtr<SessionStreamBinder> SessionStreamBinderPtr;
	typedef IntrusivePtr<ListenerBinder> ListenerBinderPtr;
	typedef IntrusivePtr<ListenerBinding> ListenerBindingPtr;
	typedef IntrusivePtr<SessionShortcut> SessionShortcutPtr;
	typedef IntrusivePtr<Multiplexer> MultiplexerPtr;

	typedef IntrusivePtr<AnyLoopReaction> AnyLoopReactionPtr;
	typedef IntrusivePtr<AnyStreamReaction> AnyStreamReactionPtr;	
	typedef IntrusivePtr<AnyListenerReaction> AnyListenerReactionPtr;
	typedef IntrusivePtr<AnyMultiplexerReaction> AnyMultiplexerReactionPtr;
}

/**
 * @}
 */

#endif//VCE_VCE2STREAM_H
