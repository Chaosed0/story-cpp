

=== Fall ===
#img=1-1-paloma-door
#amb=fall-outdoor-ambience

In front of you is a rather imposing door, nearly twice your height. Paint peels off of it. Upon the door there is a conspicuously large knocker in the shape of a bull-head, and a sign saying "Paloma - Common Room".


- (at_door)

 * [Use the knocker.] -> use_knocker
 * [Go through the door without knocking.] -> use_door
 * [Make sure that you're in the right place.]
 You uncrumple the piece of paper in your in your pocket. "Paloma Dorms", it says. Seems right to you. -> at_door
 
= use_knocker
You reach for the knocker. Before you have a chance, it opens its eyes, and blinks at you. It levitates upward and bangs itself against the door three times. A bit of astral dust sparkles in the air around the knocker.

Ah, well, you are in the city now. You should have expected magic would be commonplace.

You wait at the door with your bags. About twenty seconds pass. You shuffle your feet and wonder if you should knock again. But just then, the door opens with a squeal.

You see a face peeking out, framed by long hair. "Hello?"

 * "Um, hi."[] You're unsure about who this is. She seems young, about your age.
 
"Are you, um, another student?" She asks.
 
 You nod, and she blinks owlishly in return. "Well, come in."
 
 * "Hello!"[] You respond with a smile.
 The girl smiles back uncertainly. "Um, are you also another student?"
 
 You respond enthusiastically, with a yes. She opens the door a bit wider. "Well, come in."
 
 - 
 
 * [Follow her into the common room.] You follow the girl. The door closes behind you.
 
 -> in_common_room
 
 = use_door
 
 There is no doorknob on the door, nor any other indication on how to open it. For a moment, you puzzle over how to enter.
 
 You hesitantly touch the door. As soon as your fingers brush it, the door creaks open slowly, startling you. A little bit of astral dust sparkles around the door.
 
 Ah, right. You're in the city now. You should have expected magic would be commonplace.
 
 * [Walk through.] You proceed through the threshold. The door closes by itself behind you.
 
  -> in_common_room
 
 = in_common_room
 
 #img=1-2-paloma-room
 #amb=fall-indoor-ambience
 You stand in a rectangular room. Magelight shines from sconces onto ancient pilled-up carpet. You see the fading sun through cloudy windows. A sagging shuffleboard table sits at one side of the room, and on the other, a fireplace. The room feels tired, but well kept.
 
 There are several comfortable chairs pulled up around the fire, and in these chairs sit {use_knocker: two|three} folks. {use_knocker: The girl you met at the door walks around one of the chairs and sits down.}
 
 The boy on the left waves. "Ah, another student! Come in, come in - we are just introducing ourselves!"
 
 * [Take a seat at the fire.]
 
 - You take a seat at the fire, between the two girls. {use_knocker: The girl who opened the door for you sits on your right.}
 
 The boy on your left starts saying something, but is interrupted by the girl next to him. "Hello! Another new face. We're all first years here - I assume you are too?"
 
 You nod, and she continues. "Alexis is my name, but call me Alex."
 
 Alex points over at the girl on your right, who smiles shyly. "That's Mia - " she points at the boy to your right " - and that's... Cren."
 
 Cren scowls at Alex, clearly put out by her short introduction. He stands up and bows. "Crechenallan, of the royal Moran family, the protectors of the north, the hands of justice, loyal vassals of the king."
 
 "I come from a long line of great Morans, each one greater than the last. My father, and my grandfather before me, have both served the throne for many decades, and, stars align, we shall for many more..."
 
 - (cren_talk_opts)
 
 + {cren_talk_opts < 6} [Allow Cren to continue.]
 
 {cren_talk_opts == 1: "... In fact, my great-great-great-great-grandfather was present at the founding of the kingdom itself! When the first flag was planted upon virgin soil, when Harold Moran the First raised the first stone hall..."}
 
 {cren_talk_opts == 2: "...And I, as the third-born son, shall naturally continue the tradition of service. As my tutor says, tradition is the greatest virtue of all! It is the only way our society carries on, through the wisdom of our elders. Of course..."}
 
 {cren_talk_opts == 3: "...New blood has its place too, and that is why I am here to attend Blakeley college. To learn what I can from those greater than I, and - " Cren gestures at you all, and Mia sits up a bit straighter - "from my peers, as well, whom I am certain each have their own merits."}
 
 {cren_talk_opts == 4: "...I am certain each of us has a place to play in the future! That's what my tutor says. Even the meanest scullery-maid has a part to play. Why, I remember when I first met my tutor, wise man that he is. It was when I was but a wee lad of six..."}
 {cren_talk_opts == 4: You notice Alex's eyes start to glaze over.}
  
 {cren_talk_opts < 5: -> cren_talk_opts}
 
 {cren_talk_opts == 5: "My father brought me to the audience hall, where my tutor was waiting, clad in resplendent robes of the second-finest silk. There, he spake the wisest words I have ever heard in my short life, and likely will ever hear. He said -"}
 {cren_talk_opts == 5: Alex rolls her eyes, then leans back in her chair and lets out a loud yawn. Mia stifles a giggle, and Cren glares at her.}
 
 * [Yawn loudly and conspicuously.]
 You crack your jaws open and let out a very loud yawn. Alex stifles a snort, and Mia lets out a tiny giggle. Cren glares at you.
 
 - Before Cren has a chance to resume, Alex jumps on the opportunity. "Enough about Moran!" She pronounces it like "Moron".
 
 Cren opens his mouth to retort, but Alex turns to you and asks:
 
 "So, what's your name?"
 
 * [INPUT: player_name]
 
 - Alex leans over, from her chair, and offers you her hand. You shake it. "Nice to meet you, {player_name}. Before you {use_knocker: knocked|came in} - and before Cren's long ramble - I mean, introduction - I was just asking Mia about how she got into Blakeley."
 
 "Oh, yes!" Mia starts. "Where I come from, there's a program the government sponsors to send students here. I applied, the stars aligned, and, well, I got in."
 
 * (silent_1) [Allow her to continue.]
 Everyone pauses for a moment, but it seems she's done speaking.
 
 Alex prompts her. "Those kinds of programs are... rather competitive, no? Comes with a scholarship, and all that?"
 
 Mia looks down and shrugs. "Oh, I don't know. I have always gotten good grades, I suppose, but Tyvia is desperate for mages. Or, um, that's what my parents say."
 
 * [Ask where she's from.]
 
 You ask which country she's from. Mia looks down. "I'm from Tyvia. It's a small country to the west."
 
 - "Tyvia!" Cren taps his chin. "Old Vancian country. I would wager that most of our classmates could not identify it on a map. Rather interesting that they're sending their brightest to Blakeley. Ambitions of arcanization, eh?"
 
 Mia nods hesitantly. "Um, yeah. We still import, like, sixty percent of our astral dust from the Vance Union. I think that's why they're desperate for mages. That's what my parents say." {silent_1: Mia seems to realize she's repeating herself, and trails off.} She glances at you.
 
 * [Continue]
 
 - "How about you, um, {player_name}? Why'd you join Blakeley?"
 
 You joined Blakeley University of the Arcane...
 
 * [To find a good job.]
 ~ player_blakeley_motivation = "find_job"
 You joined Blakeley University to find a good job.
 
 Alex nods. "That's fair. Mages have great job opportunities. No better career, these days."
 
 * [To help people.]
 ~ player_blakeley_motivation = "help_people"
 You joined Blakeley University to help people.
  
 Alex eyes you, one eye arched. "How noble. Simple as that, eh? Learn magic and help the world?"
 
 * [To create great art.]
 ~ player_blakeley_motivation = "create_art"
 You joined Blakeley University to create great art.
 
 Alex scoffs a little at that, but doesn't say anything.
 
 * [To develop the technomancy of the future.]
 ~ player_blakeley_motivation = "develop_technomancy"
 You joined Blakeley University to develop the technomancy of the future.
 
 Alex thins her lips. "The future, huh?"
 
 * [Because it just seemed like the next logical step after secondary school.]
 You joined Blakeley University because it just seemed like the thing to do after secondary school.
 
 Alex snorts. "Joining the most prestigious arcane university because it's the thing to do? You must be as smart as Mia."
 
 - Cren thumps his fist on his chair. "Any reason is a fine reason, says I! It is not only Tyvia that shall need mages. The entire world has got plenty of problems to solve, and we - the people in this very room, the brightest of the bright - shall be the future leaders of this world!"
 
 // Need a lot more time to develop the world between this and The Drift
 
 "Tsch." Alex shakes her head and looks at the ceiling. "As if there's going to be a future to lead."
 
 * [Nod in agreement. You know what she's referring to.]
 You nod. You say simply, "Star Drift."
 
 * (unsure) You're unsure where she's going with this.
 Cren, too, is taken aback somewhat by this statement. "No future? What do you mean?"
 
 Alex's mouth twists. "Don't tell me you're a drift denier, Cren."
 
 -
 * [...]
 
 #img=1-3-stars
 #amb=
 - It's gotten worse over the past few years. The drift is getting more noticeable. Constellations are moving out of their proper alignment. It's hard to tell, but on a dark day, older folks say they can tell the stars are getting dimmer.
 
 And most alarming - the sun is growing smaller. It, too, drifts away from the earth, as if it can no longer stand our presence in the cosmos.
 
 Astromancers predict that, if trends continue, in the next thirty to sixty years, there may be a tipping point at which drifting away from the sun's orbit may become inevitable.
 
 * [...]
 
 #img=1-2-paloma-room
 #amb=fall-indoor-ambience
 - Cren sits up. "{unsure: No, I'm not denying it at all. Star drift is a problem. |Yes, it is undeniably a problem.} Clearly, this is one reason for which we are in this school to begin with! Surely we will find ways to solve it."
 
 Alex narrows her eyes. "Most people just want to pretend it's going to go away. How can we solve something like that?"
 
 * [Continue]
 
 - Alex continues, staring at Cren. "You know, I read something that says Moran generates twenty-five percent of the world's astral dust. Maybe the better question is, how are <i>you</i> going to solve it?"
 
 Cren blinks. "Well, um." He shifts uncomfortably as you all look at him. "Ahem. As you know, Moran is the second-largest consumer of astral dust. Our astral drills are an important part of the world economy -"
 
 Alex snorts. "Helpless, just like I thought."
 
 Cren blushes. "It is not that simple! We have people to take care of. Without astral dust, they cannot live their lives. Everything runs on it. It is the foundation of our food, our defense strategy..."
 
 Alex waves her hand. "Just like all the nobility - always with an excuse."
 
 * [Change the subject.]
 In an effort to change the subject, you quickly remind Alex that she hasn't said yet why she joined Blakeley. 
 
 * (silent_2) [Stay silent.]
 Cren repeats, heatedly: "It is not that simple! And it is not an excuse. Problems like this are exactly why I'm here - I wish to lead people into the future! And what about you, anyway? You never told us why you joined Blakeley."
 
 - Alex's lips twitch into a half-smile. "Isn't it obvious? To shut up know-it-alls like {silent_2: you|Cren}."
 
 Cren sputters a bit. At this, Alex lets out a laugh, her smile easing up a bit. "Ah, I'm sorry Cren, maybe that was undeserved."
 
 Alex stands up and holds out her hand. Cren grabs it, somewhat uncertainly, and gives it a shake. Alex says, "Listen, I can tell you're a good person - mostly, anyway. It's the first day of school. Maybe we should ease up a bit. How about we go and play a round of shuffleboard?"
 
 "Ah..." It's obvious Cren's still struggling a bit, but he ends up choosing the side of grace. He stands up. "Oh... Alright. I used to play in the hall all the time with the servants..."
 
 * [Continue]
 
 - Mia looks up, shyly. "I'd like to join, but I, um, don't know how to play though."
 
 At this, Cren brightens. "Oh, I'll teach you! Let me tell you about the time I played against the best shuffleboard player in Moran..." He rambles on. This time, Alex rolls her eyes but lets him continue.
 
 Naturally, you join shuffleboard too, with your new friends. After all...
 
 * We've got a hundred years to figure out star drift.
 * The only way we're going to solve the drift is together.
 * Better to enjoy the now than to worry about the future.
 * You're sure that people smarter than you are working on it.
 
 -
 
 * [Time passes.]
 
 -> DONE
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 