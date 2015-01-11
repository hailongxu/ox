

/**
	2008-7-20 
	a method how to judge whether the current system is little endian or big endian
*/

#pragma once

namespace ox
{
	namespace utl
	{
		/**
			Big endian:
				��Ϊ��һ���ֽ������λ�ֽ�
				���մӵ͵�ַ���ߵ�ַ��˳�������ݵĸ�λ�ֽڵ���λ�ֽ�
			Little endian���෴:
				����Ϊ��һ���ֽ������λ�ֽ�
				���մӵ͵�ַ���ߵ�ַ��˳�������ݵĵ�λ�ֽڵ���λ�ֽ�
		*/

		class endianness
		{
		public:
			enum { __little_endian = 0, __big_endian = 1 };
			static unsigned char test()
			{
				test_struct endian_test =
				{
					{
						__little_endian, // frist byte
						-1, // second byte
						-1, // third byte
						__big_endian // fourth byte
					}
				};
				return endian_test.c[0]/*reinterpret_cast<char>(endian_test)*/;
			}
			static bool is_big_endian() { return test()==__big_endian; }
			static bool is_little_endian() { return test()==__little_endian; }
		private:
			union test_struct
			{
				char c[4];
				unsigned long l;
			};
		};

		int SetEndian()
		{
			int isLittleEndian = -1; /// unknown
			unsigned char *p = NULL;
			unsigned short tst = 0x1234;

			/* ��ȡCPU�ֽ���˳�� */
			p = (unsigned char *)&tst;

			if (*p == 0x12) isLittleEndian = 1;
			else if(*p == 0x34) isLittleEndian = 0;
			else
			{
				//fprintf(stderr, "Unknown CPU Bytes Order!\n");
				return -1;
			}

			return 0;
		}
	}
}

