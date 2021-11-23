import React from "react";
import styled from "styled-components";

import ChatLogItem from "./ChatLogItem";


function ChatLogTemplate({ chats }) {

  // chats 배열을 받아 저장해 둔 채팅 로그를 보여줌
  // ChatLogItem 컴포넌트 사용
  return (
    <ChatContainer>
      <h1 style={{ color: "black" }}>Chat Log</h1>
      {chats.map((chat) => {
        return <ChatLogItem chat={chat} />
      })}
    </ChatContainer>
  );

}

const ChatContainer = styled.div`  
  width: 500px;
  margin: 10px;
  padding: 20px;
  border-radius: 1%;
  box-shadow: 2px 2px 2px 2px gray;
  background-color: #f3eaaf;
  // 해당 영역 모서리를 둥글게
  // 해당 영역 모서리에 그림자
  // 해당 영역의 배경색 변경
  // 8주차 실습 구현
`;

export default ChatLogTemplate;