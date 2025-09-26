# ConsoleMiniGame
ConsoleMiniGame

1. 개요 및 전체 특징
- 콘솔 애플리케이션은 main에서 한국어 로캘을 설정하고 GameApplication을 구동해 초기화와 메인 루프를 수행합니다.

- GameApplication은 입력 처리, 업데이트, 렌더링, 종료 정리를 세분화하고, GameMaster가 제공하는 서비스들을 호출하는 게임 루프의 허브입니다.

- GameMaster는 싱글턴으로 입력·시간·콘텐츠·HUD 서비스를 소유하고 화면 서비스 포인터를 주입받아 전역 상태를 관리합니다.

- ScreenService는 더블 버퍼 기반의 콘솔 출력, 창 크기 고정, 한글 등 전각 문자의 폭 보정 등을 담당해 깨끗한 화면 전환을 구현합니다.

- InputService는 키별 콜백 바인딩과 0.1초 디바운스를 제공해 키 반복을 제어하며, 콘텐츠 전환 시 바인딩을 재구성합니다.

- TimeService는 60 FPS 기반의 틱 간격을 계산해 업데이트 타이밍을 제어합니다.

- HUD는 외곽 프레임, 본문 패널, 명령 패널 레이아웃을 관리하며 텍스트 큐와 커맨드 라인 출력을 지원합니다.



2. 컨텐츠 구성
- ContentManager는 타이틀, 스네이크, 던전 콘텐츠를 로드하고 전환 시 현재 콘텐츠의 해제·초기화와 입력/HUD 리셋을 수행합니다.


2.1 Title
- TitleContent는 메뉴 항목(스네이크, 던전, 종료)을 구성해 키보드 내비게이션 및 선택 콜백을 바인딩하고, HUD에 아스키 아트와 안내 문구를 출력합니다.


2.2 THE SNAKE
-  SnakeContent는 기본 보드 크기·이동 속도를 설정하고 방향 전환, 재시작, 타이틀 복귀 입력을 바인딩합니다.

-  SnakeGameState는 뱀 머리 이동, 충돌 판정, 먹이 생성, 점수 계산을 처리하며, 이동 주기를 시간 기반으로 제어합니다.

-  SnakeRenderer는 HUD가 제공하는 뷰포트에 보드, 경계선, 뱀/먹이, 점수판을 그립니다.


2.3 DUNGEON
- DungeonContent는 던전 이동·재시작·타이틀 복귀 입력을 묶고, HUD 레이아웃을 변경한 뒤 게임 상태를 초기화합니다.

- DungeonGameState는 확률 기반 맵 생성과 L자 통행 경로 확보, 이동 가능 여부, 아이템/몬스터/출구 이벤트, 턴제 전투를 담당합니다.

- 전투는 BattleCharacter 기반으로 공격력·방어력·민첩도를 사용해 피해량과 턴 순서를 결정하며, 플레이어/몬스터 파생 클래스가 참여합니다.

- DungeonRenderer는 전각 문자를 활용해 맵 타일과 플레이어 위치, 상태 메시지를 출력합니다.



3. 클래스 트리
   
main
└─ GameApplication
   ├─ ScreenService
   └─ GameMaster (Singleton)
      ├─ InputService
      ├─ TimeService
      ├─ HUD
      └─ ContentManager
         ├─ TitleContent
         ├─ SnakeContent
         │  ├─ SnakeGameState
         │  └─ SnakeRenderer
         └─ DungeonContent
            ├─ DungeonGameState
            │  ├─ DungeonPlayer (BattleCharacter)
            │  ├─ DungeonMonster (BattleCharacter)
            │  └─ 기타 맵/전투 로직
            └─ DungeonRenderer


위 구조는 각 헤더 선언과 소유 관계를 기반으로 작성했습니다.



4. 클래스별 역할
- IContent: 모든 콘텐츠가 구현해야 하는 초기화·해제·업데이트·렌더링 인터페이스입니다.

- GameApplication: 게임 실행 수명주기를 캡슐화해 입력/업데이트/렌더링 루프와 종료 처리를 담당합니다.

- GameMaster: 싱글턴으로 주요 서비스와 현재 화면을 관리하고, 종료 플래그를 제어합니다.

- ScreenService: 콘솔 버퍼 생성, 커서 제어, 색상 속성, 전각 문자 폭 처리, 이중 버퍼 스왑을 수행합니다.

- InputService: 키 코드와 콜백을 매핑하고, 디바운스 로직을 포함한 업데이트 루틴과 맵 클리어 기능을 제공합니다.

- TimeService: 이전 틱 시각을 기억해 60 FPS 간격으로 업데이트 가능 여부를 반환합니다.

- HUD: 프레임/패널 렌더링, 텍스트 큐, 명령 입력 라벨을 관리하며 레이아웃 토글과 초기값을 제공합니다.

- ContentManager: 콘텐츠 등록·전환·업데이트·렌더링 호출을 중앙집중화하고 전환 시 입력과 HUD를 리셋합니다.

- TitleContent: 메뉴 항목과 입력을 구성해 다른 게임으로 이동하거나 게임을 종료하는 타이틀 화면을 구현합니다.

- SnakeContent: 스네이크 게임 상태/렌더러를 초기화하고, 방향·재시작·타이틀 복귀 입력을 처리합니다.

- SnakeGameState: 뱀 이동, 충돌, 먹이 스폰, 점수 및 게임 오버 상태를 관리합니다.

- SnakeRenderer: 보드, 경계, 뱀/먹이, 점수 및 상태 UI를 콘솔에 출력합니다.

- DungeonContent: 던전 탐험 상태/렌더러를 초기화하고 입력 및 타이틀 복귀 요청을 제어합니다.

- DungeonGameState: 맵 생성, 이동 검증, 이벤트 처리, 몬스터와의 전투, 탈출 판정을 수행합니다.

- DungeonRenderer: 던전 타일과 플레이어, 상태 텍스트, 결과 메시지를 렌더링합니다.

- BattleCharacter 및 파생(DungeonPlayer, DungeonMonster): HP·능력치 기반 전투 로직과 치유/공격 역할을 정의합니다.
