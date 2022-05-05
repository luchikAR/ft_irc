# ft_irc

Сабджект:
1. Вы должны иметь возможность проходить аутентификацию, устанавливать nickname, username, присоединяться к каналу,
отправлять и получать личные сообщения с помощью вашего клиента.
2. Все сообщения, отправленные от одного клиента на канал, должны быть переадресованы
каждому другому клиенту, присоединившемуся к каналу.
3. У вас должны быть операторы и постоянные пользователи.
4. Затем вы должны реализовать команды, специфичные для операторов.

Необходимо протестировать:
1. PRIVMSG
2. NOTICE
2. JOIN - каверзные тесты (join уже в канале?) (история сообщений в канале?)
3. KICK - каверзные тесты (кикнуть сам себя?)

Баги:
1. 

Проверенные команды:
1. Переделанны/исправленны/протестированны commands: PASS NICK USER
2. JOIN / KICK - работают, но каверзных тестов не было.
