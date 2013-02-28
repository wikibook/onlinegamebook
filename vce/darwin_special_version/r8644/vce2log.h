#ifndef VCE_VCE2LOG_H
#define VCE_VCE2LOG_H

#include "vce2.h"

namespace vce
{
	//!@ja ���O�̎�� @endja
	enum VCE_LOG_TYPE
	{
		VCE_LOG_TYPE_START,

		VCE_LOG_TYPE_SOCKET_REUSE_ADDRESS,
		VCE_LOG_TYPE_SOCKET_SET_NONBLOCKING,
		VCE_LOG_TYPE_SOCKET_ACCEPT_TCP_SOCKET,
		VCE_LOG_TYPE_SOCKET_BIND_SOCKET,
		VCE_LOG_TYPE_SOCKET_CLOSE_SOCKET,
		VCE_LOG_TYPE_SOCKET_CONNECT_SOCKET,
		VCE_LOG_TYPE_SOCKET_CREATE_TCP_SOCKET,
		VCE_LOG_TYPE_SOCKET_GET_LOCAL_ADDRESS,
		VCE_LOG_TYPE_SOCKET_LISTEN_SOCKET,
		VCE_LOG_TYPE_SOCKET_RECV_TCP_SOCKET,
		VCE_LOG_TYPE_SOCKET_SEND_TCP_SOCKET,
		VCE_LOG_TYPE_SOCKET_SET_NODELAY,
		VCE_LOG_TYPE_SOCKET_SHUTDOWN_RECV_SOCKET,
		VCE_LOG_TYPE_SOCKET_SHUTDOWN_SEND_SOCKET,
		VCE_LOG_TYPE_SOCKET_SHUTDOWN_SOCKET,
		VCE_LOG_TYPE_SOCKET_CREATE_UDP_SOCKET,
		VCE_LOG_TYPE_SOCKET_GET_SEND_QUEUE_SIZE,
		VCE_LOG_TYPE_SOCKET_SEND_TO,
		VCE_LOG_TYPE_SOCKET_RECV_FROM,
		VCE_LOG_TYPE_SOCKET_SET_MULTICAST_INTERFACE,
		VCE_LOG_TYPE_SOCKET_POLL_CHECK,

		VCE_LOG_TYPE_CODEC_SENDED_PING,
		VCE_LOG_TYPE_CODEC_RECEIVED_PING_AND_SENDED_PONG,
		VCE_LOG_TYPE_CODEC_RECEIVED_PONG,
		VCE_LOG_TYPE_CODEC_TERMINATED,
		VCE_LOG_TYPE_CODEC_TIMEOUT,
		VCE_LOG_TYPE_CODEC_SENDED_PUBLIC_KEY,
		VCE_LOG_TYPE_CODEC_GENERATED_PRIVATE_KEY,
		VCE_LOG_TYPE_CODEC_GENERATED_PUBLIC_KEY,
		VCE_LOG_TYPE_CODEC_TIMEOUT_KEY_EXCHANGE,
		VCE_LOG_TYPE_CODEC_RECEIVED_PUBLIC_KEY,
		VCE_LOG_TYPE_CODEC_GENERATED_CRYPTIC_SHARED_KEY,
		VCE_LOG_TYPE_CODEC_RECEIVED_CRYPTIC_SHARED_KEY,
		VCE_LOG_TYPE_CODEC_SENDED_ENCRYPTION_KEY,
		VCE_LOG_TYPE_CODEC_SENDED_DECRYPTION_KEY,
		VCE_LOG_TYPE_CODEC_RECEIVED_ENCRYPTION_KEY,
		VCE_LOG_TYPE_CODEC_RECEIVED_DECRYPTION_KEY,
		VCE_LOG_TYPE_CODEC_WAITING_KEY_EXCHANGE,
		VCE_LOG_TYPE_CODEC_INVALID_CODEC,

		VCE_LOG_TYPE_LISTENER_CREATED,
		VCE_LOG_TYPE_LISTENER_RETRY,
		VCE_LOG_TYPE_LISTENER_ACCEPTED_SESSION,
		VCE_LOG_TYPE_LISTENER_UDP_RECEIVED_SYN,
		VCE_LOG_TYPE_LISTENER_UDP_SENDED_SYNACK,
		VCE_LOG_TYPE_LISTENER_UDP_RECEIVED_SYNACKACK,
		VCE_LOG_TYPE_LISTENER_SENDED_RECONNECTION_REQUEST,
		VCE_LOG_TYPE_LISTENER_OVER_BACKLOG,
		VCE_LOG_TYPE_LISTENER_DESTROYED,

		VCE_LOG_TYPE_SESSION_CREATED,
		VCE_LOG_TYPE_SESSION_RECV_BUFFER_EMPTY,
		VCE_LOG_TYPE_SESSION_FORCE_CLOSE,
		VCE_LOG_TYPE_SESSION_CLOSE,
		VCE_LOG_TYPE_SESSION_WAITING_TO_CLOSE_BECAUSE_BUFFER_EMPTY,
		VCE_LOG_TYPE_SESSION_TRYING_NAME_RESOLUTION,
		VCE_LOG_TYPE_SESSION_COMPLETE_NAME_RESOLUTION,
		VCE_LOG_TYPE_SESSION_CONNECTION_CANCELED,
		VCE_LOG_TYPE_SESSION_CONNECTION_TIMEOUT,
		VCE_LOG_TYPE_SESSION_CONNECTION_RETRIED,
		VCE_LOG_TYPE_SESSION_CONNECTION_RESTARTED,
		VCE_LOG_TYPE_SESSION_CHECKING_CONNECTION_REFUSING,
		VCE_LOG_TYPE_SESSION_CONNECTION_REFUSED,
		VCE_LOG_TYPE_SESSION_CONNECTSOCKET_IGNORED_ERROR,
		VCE_LOG_TYPE_SESSION_RECV_FUNC_FAILED,
		VCE_LOG_TYPE_SESSION_CONNECTION_COMPLETE,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_SYN,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_SYNACK,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_SYNACKACK,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_RECONNECTION_REQUEST,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_HANDSHAKING_PING,
		VCE_LOG_TYPE_SESSION_UDP_COMPLETE_HANDSHAKING,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_HANDSHAKING_PING_AND_SENDED_PONG,
		VCE_LOG_TYPE_SESSION_UDP_TIMEOUT_RECEIVING,
		VCE_LOG_TYPE_SESSION_UDP_CONNECTION_COMPLETE,
		VCE_LOG_TYPE_SESSION_UDP_CONNECTED_SEQUENCE,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_CLOSE_COMMAND,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_CLOSE_COMMAND,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_PING,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_PING,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_DATA,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_DATA,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_UNRELIABLE_DATA,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_UNRELIABLE_DATA,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_FLAGMENT_COMMAND,
		VCE_LOG_TYPE_SESSION_UDP_RECEIVED_FLAGMENT_COMMAND,
		VCE_LOG_TYPE_SESSION_UDP_SENDED_ACK,
		VCE_LOG_TYPE_SESSION_DESTROYED,

		VCE_LOG_TYPE_VCE_CREATED,
		VCE_LOG_TYPE_VCE_POLLING,
		VCE_LOG_TYPE_VCE_OS_IS_USING_PORT,
		VCE_LOG_TYPE_VCE_SELECT_ALGORITHM_CALLBACK_IS_NULL,

		VCE_LOG_TYPE_VCE_POLLING_TIME_IS_TOO_LONG,
		VCE_LOG_TYPE_TIME_OF_CLOSING_SOCKET_IS_TOO_LONG,

		VCE_LOG_TYPE_START_DNS_THREAD,
		VCE_LOG_TYPE_END_DNS_THREAD,

		VCE_LOG_TYPE_PROXY_LISTENER_CATCHED_EXCEPTION_ON_CONNECTION_TO_PROXYTOOL,
		VCE_LOG_TYPE_PROXY_LISTENER_CONNECTED_TO_PROXYTOOL,
		VCE_LOG_TYPE_PROXY_LISTENER_CLOSED_CONNECTION_PROXYTOOL,

		VCE_LOG_TYPE_UPNP_PREP,
		VCE_LOG_TYPE_UPNP_CHECK_ROUTER,
		VCE_LOG_TYPE_UPNP_CONNECTING_ROUTER_PREP,
		VCE_LOG_TYPE_UPNP_CONNECTING_TO_ROUTER,
		VCE_LOG_TYPE_UPNP_GET_ROUTER_SPEC,
		VCE_LOG_TYPE_UPNP_DISCONNECT_FROM_ROUTER,
		VCE_LOG_TYPE_UPNP_COMPLETE_SPEC,
		VCE_LOG_TYPE_UPNP_ACTION_CONNECTING,
		VCE_LOG_TYPE_UPNP_ACTION_WAIT_RESPONSE,
		VCE_LOG_TYPE_UPNP_ACTION_COMPLETE,
		VCE_LOG_TYPE_UPNP_FAIL,
		VCE_LOG_TYPE_UPNP_UNKNOWN_SEQUENCE,
		VCE_LOG_TYPE_UPNP_EXCEPTION,
		VCE_LOG_TYPE_UPNP_FAILED_RESPONSE,
		VCE_LOG_TYPE_UPNP_MAYBE_NOT_SUPPORT_PORTMAPPING,
		VCE_LOG_TYPE_UPNP_MAYBE_NOT_SUPPORT_QOS,

		VCE_LOG_TYPE_HTTP_SESSION_FAILED_TO_OPEN_FILE,
		VCE_LOG_TYPE_HTTP_SESSION_SUCCEEDED_TO_OPEN_FILE,
		VCE_LOG_TYPE_HTTP_LISTENER_ADDED_AUTHOR,

		VCE_LOG_TYPE_STREAM_MULTIPLEXER_PORT_IS_ALREADY_LISTENED,
		VCE_LOG_TYPE_STREAM_MULTIPLEXER_PORT_IS_NOT_LISTENED,
		VCE_LOG_TYPE_STREAM_MULTIPLEXER_WILD_IS_ALREADY_LISTENED,
		VCE_LOG_TYPE_STREAM_MULTIPLEXER_MAINSTREAM_IS_NOT_BOUND,
		VCE_LOG_TYPE_STREAM_MULTIPLEXER_MESSAGE_CANNOT_FORMATTED,
		VCE_LOG_TYPE_STREAM_MULTIPLEXER_MUXFLOWCONTROL_CANNOT_FORMATTED,
		VCE_LOG_TYPE_STREAM_MULTIPLEXER_PURGE_CLOSED_INTERNAL_ERROR,
		VCE_LOG_TYPE_STREAM_MULTIPLEXER_HAS_NO_LISTENER,
		VCE_LOG_TYPE_STREAM_CONNECTION_TO_PORT_IS_REJECTED,
		VCE_LOG_TYPE_STREAM_UNEXPECTED_MESSAGE_TO_MISSING_PORT,
		VCE_LOG_TYPE_STREAM_RECEIVED_MESSAGE_TO_CLOSE_PORT,
		VCE_LOG_TYPE_STREAM_MESSAGE_CONTENT_IS_BROKEN,

		VCE_LOG_TYPE_END
	};

	struct LogStructContext;

	/*!@ja
	���O���󂯎��R�[���o�b�N�֐��̌^�ł��B
	@param type ���O�̎��
	@param context ���O�̓��e�B���O�̎�ނɂ���ē��e���Ⴂ�܂��B
	@endja
	*/
	typedef void(*LogCallbackFunc)(VCE_LOG_TYPE type, LogStructContext * context);

	/*!@ja
	���O���󂯎��R�[���o�b�N�֐���ݒ肵�܂��B
	@param callback �ݒ肷��֐�
	@endja
	*/
	void SetLogCallback(LogCallbackFunc callback);

	/*!@ja
	�ߋ��̃��O�ݒ�̌݊�����ۂ������O���o�͂���R�[���o�b�N�֐��ł��B
	vce::SetLogCallback() �ł��̊֐���ݒ肵�Ă��������B
	Windows, Linux, Solaris �łł͂��̊֐��̓f�t�H���g�Őݒ肳��Ă��܂����A
	�g�ݍ��ݔłł͎g�p�������Ƃ��ɐݒ���s���Ă��������B�ݒ���s��Ȃ��ꍇ�̓��O�R�[���o�b�N�͂���܂���B
	@sa vce::SetLogCallback()
	@endja
	*/
	void DefaultLogCallbackFunc(vce::VCE_LOG_TYPE type, LogStructContext * context);

	/*!@ja
	���O�̕����񉻂��s���܂��B
	@param type ���O�̎��
	@param context ���O�̓��e
	@endja
	*/
	vce::g_string ToStringLogCallback(VCE_LOG_TYPE type, LogStructContext * context);

	/*!@ja
	���O���x���̎擾���s���܂��B
	@param type ���O�̎��
	@endja
	*/
	VCE_LOGLEVEL GetLogLevel(VCE_LOG_TYPE type);

	/*!@ja
	���O�̃t�B���^���s���܂��B
	@param type ���O�̎��
	@param off true �ɂ��邱�ƂőΏۂ̃��O�̃t�B���^���s���܂��B
	@endja
	*/
	void SetLogCallbackFilter(VCE_LOG_TYPE type,bool off);

	/*!@ja
	���O�̃t�B���^�̐ݒ���擾���܂��B
	@param type ���O�̎��
	@return �t�B���^�̗L��
	@endja
	*/
	bool GetLogCallbackFilter(VCE_LOG_TYPE type);

	/*!@ja
	���O�̃t�B���^���ꊇ���čs���܂��B
	@param type_start ���O�̎�ސ擪
	@param type_end ���̒l���܂�
	@param off true �ɂ��邱�ƂőΏۂ̃��O�̃t�B���^���s���܂��B
	@endja
	*/
	void SetLogCallbackFilterRange(VCE_LOG_TYPE type_start,VCE_LOG_TYPE type_end,bool off);

	/*!@ja
	���s���x�d���̂����߂̐ݒ�Ń��O�̃t�B���^���s���܂��B
	@endja
	*/
	void SetLogCallbackFilterRecommend();


	/*!@ja
	���O�\����
	@endja
	*/
	struct LogStructContext
	{
	};

	/*!@ja
	���O�\���́iVCE�ł̈�ʓI�ȃ��O�̌`���j
	@endja
	*/
	struct LogStruct_VceGeneric
		: public LogStructContext
	{
		//!@ja VCE �I�u�W�F�N�g�ւ̃|�C���^�B���݂��Ȃ��ꍇ�� NULL�B @endja
		VCE * api;
		//!@ja �Z�b�V�����I�u�W�F�N�g�ւ̎��ʎq�B���݂��Ȃ��ꍇ�� 0�B @endja
		VUint32 session_id;
		//!@ja ���X�i�[�I�u�W�F�N�g�ւ̎��ʎq�B���݂��Ȃ��ꍇ�� 0�B @endja
		VUint32 listener_id;

		// constructor
		LogStruct_VceGeneric();
	};

	/*!@ja
	���O�̍\���́i�\�P�b�g����̈�ʓI�ȃ��O�̌`���j
	@endja
	*/
	struct LogStruct_SocketGeneric
		: public LogStruct_VceGeneric
	{
		//!@ja �Z�b�V�����⃊�X�i�[�� uID �l @endja
		vce::VUint32 uid;

		//!@ja �\�P�b�g�̔ԍ� @endja
		int socket;

		//!@ja �G���[�̔ԍ��B�l�̓v���b�g�t�H�[�����ƂɈقȂ�܂��B @endja
		int error;

		//!@ja �����E���s @endja
		bool result;

		// constructor
		LogStruct_SocketGeneric();
	};



	/*!@ja
	���O�\����
	@endja
	*/
	template<int type>
	struct LogStruct;


	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_REUSE_ADDRESS>
		: public LogStruct_SocketGeneric
	{
		//!@ja �Ďg�p���邩�ǂ��� @endja
		bool reuse;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SET_NONBLOCKING>
		: public LogStruct_SocketGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_ACCEPT_TCP_SOCKET>
		: public LogStruct_SocketGeneric
	{
		//!@ja �ڑ��v���������� @endja
		bool accepted;
		//!@ja �v���������ꍇ�̐V�����\�P�b�g�ԍ��Baccepted == false �̂Ƃ��� -1 �ƂȂ�܂��B @endja
		int newsocket;
		//!@ja �����[�g�A�h���X�Bvce::AddrToString() �ŕ�����ɕϊ��ł��܂��B @endja
		unsigned int remote_address;
		//!@ja �����[�g�̃|�[�g�ԍ� @endja
		unsigned short int remote_port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_BIND_SOCKET>
		: public LogStruct_SocketGeneric
	{
		//!@ja �o�C���h���悤�Ƃ����A�h���X�Bvce::AddrToString() �ŕ�����ɕϊ��ł��܂��B @endja
		unsigned int address;
		//!@ja �o�C���h���悤�Ƃ����|�[�g�ԍ� @endja
		unsigned short int port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_CLOSE_SOCKET>
		: public LogStruct_SocketGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_CONNECT_SOCKET>
		: public LogStruct_SocketGeneric
	{
		//!@ja �ڑ����悤�Ƃ����A�h���X�Bvce::AddrToString() �ŕ�����ɕϊ��ł��܂��B @endja
		unsigned int address;
		//!@ja �ڑ����悤�Ƃ����|�[�g�ԍ� @endja
		unsigned short int port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_CREATE_TCP_SOCKET>
		: public LogStruct_SocketGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_GET_LOCAL_ADDRESS>
		: public LogStruct_SocketGeneric
	{
		//!@ja �擾�������[�J���̃A�h���X�Bvce::AddrToString() �ŕ�����ɕϊ��ł��܂��B @endja
		unsigned int address;
		//!@ja �擾�������[�J���̃|�[�g�ԍ� @endja
		unsigned short int port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_LISTEN_SOCKET>
		: public LogStruct_SocketGeneric
	{
		//!@ja TCP ���b�X���̃o�b�N���O�� @endja
		unsigned int backlog;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_RECV_TCP_SOCKET>
		: public LogStruct_SocketGeneric
	{
		//!@ja ��M�o�b�t�@�̃|�C���^ @endja
		const void * buffer;
		//!@ja �ő��M�o�C�g�� @endja
		std::size_t max_size;
		//!@ja ��M�����T�C�Y @endja
		int received_size;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SEND_TCP_SOCKET>
		: public LogStruct_SocketGeneric
	{
		//!@ja ���M�o�b�t�@�̃|�C���^ @endja
		const void * buffer;
		//!@ja ���M����o�C�g�� @endja
		std::size_t size;
		//!@ja ���ۂɑ��M�����o�C�g�� @endja
		std::size_t sended_size;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SET_NODELAY>
		: public LogStruct_SocketGeneric
	{
		//!@ja nodelay �ݒ��L���ɂ��邩���Ȃ��� @endja
		bool enable;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SHUTDOWN_RECV_SOCKET>
		: public LogStruct_SocketGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SHUTDOWN_SEND_SOCKET>
		: public LogStruct_SocketGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SHUTDOWN_SOCKET>
		: public LogStruct_SocketGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_CREATE_UDP_SOCKET>
		: public LogStruct_SocketGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_GET_SEND_QUEUE_SIZE>
		: public LogStruct_SocketGeneric
	{
		//!@ja nodelay �擾�����L���[�̃T�C�Y @endja
		std::size_t size;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SEND_TO>
		: public LogStruct_SocketGeneric
	{
		//!@ja ���M�o�b�t�@�̃|�C���^ @endja
		const void * buffer;
		//!@ja ���M���钷�� @endja
		int length;
		//!@ja ���M��̃A�h���X�Bvce::AddrToString() �ŕ�����ɕϊ��ł��܂��B @endja
		unsigned int address;
		//!@ja ���M��̃|�[�g�ԍ� @endja
		unsigned short int port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_RECV_FROM>
		: public LogStruct_SocketGeneric
	{
		//!@ja ��M�o�b�t�@�̃|�C���^ @endja
		void * buffer;
		//!@ja ��M���钷�� @endja
		int length;
		//!@ja ��M�����T�C�Y @endja
		int received_size;
		//!@ja ��M����̃A�h���X�Bvce::AddrToString() �ŕ�����ɕϊ��ł��܂��B @endja
		unsigned int address;
		//!@ja ��M����̃|�[�g�ԍ� @endja
		unsigned short int port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_POLL_CHECK>
		: public LogStruct_SocketGeneric
	{
		// ++ ���̂�������ʂɏo��B ++

		//!@ja �\�P�b�g�̔񓯊���� @endja
		int flags;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SOCKET_SET_MULTICAST_INTERFACE>
		: public LogStruct_SocketGeneric
	{
		//!@ja �Q������}���`�L���X�g�O���[�v�̃A�h���X�Bvce::AddrToString() �ŕ�����ɕϊ��ł��܂��B @endja
		unsigned int address;
	};


	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_SENDED_PING>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_RECEIVED_PING_AND_SENDED_PONG>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_RECEIVED_PONG>
		: public LogStruct_VceGeneric
	{
		//!@ja ping �̒x������ @endja
		VUint64 lag;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_TERMINATED>
		: public LogStruct_VceGeneric
	{
		/*!
		@ja Terminate ���ꂽ���R�BTERMINATE_REASON �Ƃ��̃��[�U�g���������܂��B
		@sa TERMINATE_REASON
		@endja*/
		unsigned int reason;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_TIMEOUT>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_GENERATED_PRIVATE_KEY>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_GENERATED_PUBLIC_KEY>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_SENDED_PUBLIC_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja ���M���ꂽ���J���̓��e @endja
		vce::g_string public_key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_RECEIVED_PUBLIC_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja ��M���ꂽ���J���̓��e @endja
		vce::g_string public_key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_GENERATED_CRYPTIC_SHARED_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja �������ꂽ�Í����ς݋��ʌ��̓��e @endja
		vce::g_string shared_key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_RECEIVED_CRYPTIC_SHARED_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja ��M���ꂽ�Í����ς݋��ʌ��̓��e @endja
		vce::g_string shared_key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_WAITING_KEY_EXCHANGE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_TIMEOUT_KEY_EXCHANGE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_SENDED_ENCRYPTION_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja ���M���ꂽ�Í����p�̔閧�� @endja
		vce::g_string key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_SENDED_DECRYPTION_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja ���M���ꂽ�������p�̔閧�� @endja
		vce::g_string key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_RECEIVED_ENCRYPTION_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja ��M���ꂽ�Í����p�̔閧�� @endja
		vce::g_string key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_RECEIVED_DECRYPTION_KEY>
		: public LogStruct_VceGeneric
	{
		//!@ja ��M���ꂽ�Í����p�̔閧�� @endja
		vce::g_string key;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_CODEC_INVALID_CODEC>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_CREATED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_ACCEPTED_SESSION>
		: public LogStruct_VceGeneric
	{
		//!@ja ���F�����Z�b�V�����̃\�P�b�g�ԍ� @endja
		int newsocket;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_RETRY>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_SENDED_RECONNECTION_REQUEST>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_OVER_BACKLOG>
		: public LogStruct_VceGeneric
	{
		//!@ja �o�b�N���O @endja
		int backlog;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_DESTROYED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CREATED>
		: public LogStruct_VceGeneric
	{
		//!@ja ���̃Z�b�V�����𐶐��������X�i�[�B�����Ő��������ꍇ�� NULL �ƂȂ�B @endja
		VUint32 parent_listener_id;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_DESTROYED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_RECV_BUFFER_EMPTY>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_FORCE_CLOSE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CLOSE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_WAITING_TO_CLOSE_BECAUSE_BUFFER_EMPTY>
		: public LogStruct_VceGeneric
	{
		//!@ja �N���[�Y���J�n�����\�P�b�g�ԍ� @endja
		int socket;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_TRYING_NAME_RESOLUTION>
		: public LogStruct_VceGeneric
	{
		//!@ja ���O�������悤�Ƃ��Ă���z�X�g�� @endja
		const char * hostname;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_COMPLETE_NAME_RESOLUTION>
		: public LogStruct_VceGeneric
	{
		//!@ja ���O�����ɂ�蓾��ꂽ IP �A�h���X @endja
		vce::VUint32 address;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CONNECTION_CANCELED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CONNECTION_TIMEOUT>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CONNECTION_RETRIED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CONNECTION_RESTARTED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CONNECTION_REFUSED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CHECKING_CONNECTION_REFUSING>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CONNECTSOCKET_IGNORED_ERROR>
		: public LogStruct_VceGeneric
	{
		//!@ja ���������G���[ @endja
		int error;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_RECV_FUNC_FAILED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_CONNECTION_COMPLETE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_SYN>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_UDP_RECEIVED_SYN>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_UDP_SENDED_SYNACK>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_SYNACK>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_SYNACKACK>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_LISTENER_UDP_RECEIVED_SYNACKACK>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_RECONNECTION_REQUEST>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_HANDSHAKING_PING>
		: public LogStruct_VceGeneric
	{
		//!@ja ���M���J�n�������� @endja
		vce::VUint64 now_time;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_CONNECTION_COMPLETE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_CONNECTED_SEQUENCE>
		: public LogStruct_VceGeneric
	{
		//!@ja ���M�V�[�P���X�ԍ� @endja
		unsigned int sequence_num_send;
		//!@ja ��M�V�[�P���X�ԍ� @endja
		unsigned int sequence_num_recv;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_COMPLETE_HANDSHAKING>
		: public LogStruct_VceGeneric
	{
		//!@ja ������������ @endja
		vce::VUint64 now_time;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_HANDSHAKING_PING_AND_SENDED_PONG>
		: public LogStruct_VceGeneric
	{
		//!@ja �n���h�V�F�C�L���O PING ����M���APONG �𑗐M�������� @endja
		vce::VUint64 now_time;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_TIMEOUT_RECEIVING>
		: public LogStruct_VceGeneric
	{
		//!@ja �^�C���A�E�g�����Ƃ��̎��� @endja
		vce::VUint64 now_time;
		//!@ja �Ō�Ɏ�M�����Ƃ��̎��� @endja
		vce::VUint64 last_recv_time;
		//!@ja �^�C���A�E�g���� @endja
		vce::VUint64 timeout_time;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_CLOSE_COMMAND>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_CLOSE_COMMAND>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_PING>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_PING>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_DATA>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_DATA>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_UNRELIABLE_DATA>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_UNRELIABLE_DATA>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_FLAGMENT_COMMAND>
		: public LogStruct_VceGeneric
	{
		//!@ja �ύX����E�B���h�E�T�C�Y @endja
		unsigned short int window_size;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_RECEIVED_FLAGMENT_COMMAND>
		: public LogStruct_VceGeneric
	{
		//!@ja �ύX����E�B���h�E�T�C�Y @endja
		unsigned short int window_size;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_SESSION_UDP_SENDED_ACK>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_VCE_CREATED>
		: public LogStruct_VceGeneric
	{
		//!@ja VCE �̃o�[�W�����ԍ� @endja
		const char * version_number;
		//!@ja VCE �̃r���h�ԍ� @endja
		const char * build_number;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_VCE_POLLING>
		: public LogStruct_VceGeneric
	{
		//!@ja �O��̃��O���獡��܂ł� Poll �������� @endja
		vce::VUint32 polling_count;
		//!@ja ���� VCE �I�u�W�F�N�g���Ǘ����Ă��郊�X�i�[�̐� @endja
		int num_of_listeners;
		//!@ja ���� VCE �I�u�W�F�N�g���Ǘ����Ă���Z�b�V�����̐� @endja
		int num_of_sessions;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_VCE_OS_IS_USING_PORT>
		: public LogStruct_VceGeneric
	{
		//!@ja ���b�X�����悤�Ƃ��� OS ���g�p���̃|�[�g�ԍ� @endja
		vce::VUint16 port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_VCE_POLLING_TIME_IS_TOO_LONG>
		: public LogStruct_VceGeneric
	{
		//!@ja 1��̃|�[���ɂ����������� @endja
		vce::VUint64 time;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_TIME_OF_CLOSING_SOCKET_IS_TOO_LONG>
		: public LogStruct_VceGeneric
	{
		//!@ja �\�P�b�g�̃N���[�Y�ɂ����������ԁB
		// ���̃��O���o���ꍇ�͕��Ђɂ��񍐂���������ƍK���ł��B
		// @endja
		vce::VUint64 time;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_VCE_SELECT_ALGORITHM_CALLBACK_IS_NULL>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_PROXY_LISTENER_CATCHED_EXCEPTION_ON_CONNECTION_TO_PROXYTOOL>
		: public LogStruct_VceGeneric
	{
		//!@ja ProxyListener �� ProxyTool �Ԃł̒ʐM�Ŕ������� Exception @endja
		vce::VCE_EXCEPTION exception;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_PROXY_LISTENER_CONNECTED_TO_PROXYTOOL>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_PROXY_LISTENER_CLOSED_CONNECTION_PROXYTOOL>
		: public LogStruct_VceGeneric
	{
		//!@ja ProxyListener �� ProxyTool �Ԃł̒ʐM�Őؒf���ꂽ���R @endja
		vce::VCE_CLOSEREASON close_reason;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_PREP>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_CHECK_ROUTER>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_CONNECTING_ROUTER_PREP>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_CONNECTING_TO_ROUTER>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_GET_ROUTER_SPEC>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_DISCONNECT_FROM_ROUTER>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_COMPLETE_SPEC>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_ACTION_CONNECTING>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_ACTION_WAIT_RESPONSE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_ACTION_COMPLETE>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_FAIL>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_UNKNOWN_SEQUENCE>
		: public LogStruct_VceGeneric
	{
		//!@ja �s���ȃV�[�P���X�ԍ� @endja
		int sequence;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_EXCEPTION>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_FAILED_RESPONSE>
		: public LogStruct_VceGeneric
	{
		//!@ja HTTP �X�e�[�^�X�ԍ� @endja
		int status;

		//!@ja HTTP ���X�|���X�̓��e @endja
		const char * response;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_MAYBE_NOT_SUPPORT_PORTMAPPING>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_UPNP_MAYBE_NOT_SUPPORT_QOS>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_HTTP_SESSION_FAILED_TO_OPEN_FILE>
		: public LogStruct_VceGeneric
	{
		//!@ja �J���̂Ɏ��s�����t�@�C���� @endja
		const char * filename;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_HTTP_SESSION_SUCCEEDED_TO_OPEN_FILE>
		: public LogStruct_VceGeneric
	{
		//!@ja �J���̂ɐ��������t�@�C���� @endja
		const char * filename;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_HTTP_LISTENER_ADDED_AUTHOR>
		: public LogStruct_VceGeneric
	{
		//!@ja �ǉ������F�؃��[�U�̖{���̃f�B���N�g���� @endja
		const char * real_directory;

		//!@ja �ǉ������F�؃��[�U�̃p�X���[�h @endja
		const char * password;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_PORT_IS_ALREADY_LISTENED>
		: public LogStruct_VceGeneric
	{
		//!@ja ���łɎg���Ă��Ċ��蓖�Ă��Ȃ������|�[�g�ԍ� @endja
		vce::VUint32 port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_PORT_IS_NOT_LISTENED>
		: public LogStruct_VceGeneric
	{
		//!@ja �g�p���Ă��Ȃ��̂Ƀ��X�i�[�����菜�����Ƃ����|�[�g�ԍ� @endja
		vce::VUint32 port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_WILD_IS_ALREADY_LISTENED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_MAINSTREAM_IS_NOT_BOUND>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_MESSAGE_CANNOT_FORMATTED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_MUXFLOWCONTROL_CANNOT_FORMATTED>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_PURGE_CLOSED_INTERNAL_ERROR>
		: public LogStruct_VceGeneric
	{
		//!@ja �Ǖ��ł��Ȃ��������[�J���̃|�[�g�ԍ� @endja
		vce::VUint32 local_port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MULTIPLEXER_HAS_NO_LISTENER>
		: public LogStruct_VceGeneric
	{
		//!@ja ���X�i�[���Ȃ��������[�J���̃|�[�g�ԍ� @endja
		vce::VUint32 local_port;

		//!@ja ���X�i�[���Ȃ����������[�g�̃|�[�g�ԍ� @endja
		vce::VUint32 remote_port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_CONNECTION_TO_PORT_IS_REJECTED>
		: public LogStruct_VceGeneric
	{
		//!@ja �ڑ������ۂ��ꂽ���[�J���̃|�[�g�ԍ� @endja
		vce::VUint32 local_port;

		//!@ja �ڑ������ۂ��ꂽ�����[�g�̃|�[�g�ԍ� @endja
		vce::VUint32 remote_port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_UNEXPECTED_MESSAGE_TO_MISSING_PORT>
		: public LogStruct_VceGeneric
	{
		//!@ja �\�����Ȃ����b�Z�[�W�������������[�J���̃|�[�g�ԍ� @endja
		vce::VUint32 local_port;

		//!@ja �\�����Ȃ����b�Z�[�W���������������[�g�̃|�[�g�ԍ� @endja
		vce::VUint32 remote_port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_RECEIVED_MESSAGE_TO_CLOSE_PORT>
		: public LogStruct_VceGeneric
	{
		//!@ja �ؒf���b�Z�[�W����M�������[�J���̃|�[�g�ԍ� @endja
		vce::VUint32 local_port;

		//!@ja �ؒf���b�Z�[�W����M���������[�g�̃|�[�g�ԍ� @endja
		vce::VUint32 remote_port;
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_STREAM_MESSAGE_CONTENT_IS_BROKEN>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_START_DNS_THREAD>
		: public LogStruct_VceGeneric
	{
	};

	template<>
	struct LogStruct<VCE_LOG_TYPE_END_DNS_THREAD>
		: public LogStruct_VceGeneric
	{
	};




	void CallbackAllLog();

}


#endif
