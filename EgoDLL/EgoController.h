#pragma once
#include "include/EgoInterface.h"
#include <memory>
class CEgoClient;
class CEgoController:public IEgoControl
{
public:
	CEgoController(IEgoNotify* noitfy);
	virtual void Login(std::string account, std::string pass) override;
	//virtual void ReqCarList() override;
	virtual void Start(std::array<IRender*, RenderPosition::ALL>& ar) override;
	virtual void OnCarConnect(int32_t peer) override;
	virtual void OnCarLeave() override;
	virtual ControlStatus CheckStatus() override;
private:
	std::unique_ptr<CEgoClient> _client;
};

