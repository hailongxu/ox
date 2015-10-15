


#pragma once


___namespace2_begin(ox,ipc)


struct cmdid_helper
{
	typedef size_t uinteger;
	cmdid_helper(uinteger& _cmdid):_m_cmdid(_cmdid) {}
	void set(uinteger cmdid)
	{
		_m_cmdid = cmdid;
	}
	void set_cmdid(uinteger cmdid)
	{
		_m_cmdid = cmdid;
	}
	void set_cmdid(uinteger cmdid,bool is_not_back)
	{
		_m_cmdid = cmdid | (is_not_back?(~(uinteger(-1)>>1)):0);
	}
	uinteger get_cmdid() const
	{
		return (_m_cmdid & (uinteger(-1)>>1));
	}
	/// the highest bit is back bit flag
	bool is_not_back() const
	{
		return (_m_cmdid & (~(uinteger(-1)>>1)));
	}
	uinteger& _m_cmdid;
};

___namespace2_end()