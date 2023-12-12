
=== Summer ===
#img=4-1-stars
#amb=summer-outdoor-ambience

 A chill wind blows, shaking the nearby tree branches. It's unseasonably cold for the summertime. You draw your jacket a bit tighter, and lean closer to the stone wall.
 
 The stars twinkle overhead, bright against the night sky. Your last day at Blakeley for the year happens to coincide with the night of a meteor shower.
 
 - (opts_look)
 
 * [Look up at the sky.] You look at the sky and wait a few minutes.
 
 A streak of light flashes across your vision. Your eyes track it as Mia extends an arm excitedly and points. "Oh, there!"
 
 Cren has an arm around Mia. He grins at her. "Shall we make a wish?"
 
 She looks at him, then blushes and looks away. When she looks back, she has a wicked look on her face. "You know what I wish."
 
 At this, Cren simply laughs.
 -> opts_look
 
 * (look_outside) [Look over the surrounding landscape.]
 You're sitting on a hill next to a stone wall - some long-gone farmer's fence, perhaps. The magefire in front of you illuminates the grassy hillside. In the distance, you can make out the silhouettes of a few trees.
 
 Somewhere in the trees is a small trail, leading to a small town, and in the town is the railcar that took you here.
 -> opts_look
 
 * [Look over at Alex.] She gazes up at the sky, a faraway look on her face.
 
 ** [Tap her on the shoulder.] You tap Alex on the shoulder, breaking her reverie.
 
 She looks over. Her face looks very relaxed. You ask her what she's thinking.
 
 "Oh... A lot of things." She smiles a half smile. "Like how little I'm gonna miss you all this summer."
 
 She goes back to looking at the sky. The smile slips off her face as the faraway look returns.
 -> opts_look
 
 ** [Don't interrupt her thoughts.] You leave Alex alone with her thoughts.
 -> opts_look
 
 * ->
 
 - The small {TURNS_SINCE(-> look_outside) > 0: magefire in front of you|magefire} flickers. You shiver. It's growing colder. You suppose someone ought to stoke the fire.
 
 * [Ask Alex to cast the spell.] You ask Alex to cast the spell.
 
 "I got it." Alex leans over. She takes a pinch of astral dust out of the pouch at her belt, makes a few gestures, and blows the dust at the magelight. <>
 
 * (yourself_light) [Cast the spell yourself.]
 
 You take a pinch of astral dust out of the pouch at your belt, make a few gestures, and blow the dust at the magelight. <>
 
 - It grows visibly and emits more warmth.
 
 Across the fire, Cren and Mia share a look. Cren turns away before Mia does. She shrugs.
 
 Everyone goes back to looking at the sky. A few minutes pass.
 
 * [Continue]
 
 - Alex breaks the silence. "Hey, Cren." You all look over. "Don't tell me you're still planning on wasting copious amounts of money on a <i>Sorcerous Leadership</i> degree."
 
 "Oh, come off it." Cren waves his hand. "If I am to lead Moran into the future, I might as well know what I am doing, yes?"
 
 "And anyway, it is probably a more difficult degree than Arcane Computation, whatever that is." Alex snorts, but Cren persists. "You slept through half your classes this year. Surely whatever major you pick wouldn't be that hard."
 
 Alex huffs. "Psh, shut up. Everyone knows Arcane Computation is, like, the hardest degree at the school. And besides..." She tosses her head. "All I gotta do is graduate and I'll be guaranteed a good job. Easy money, easy life."
 
 - (joke_opts)
 
 * (joke_cren) {not joke_alex} [You make a joke at Cren.] You smile at Cren. What's sorcerous about leadership, anyway?
 
 He rolls his eyes. "Yes, yes, it's a bit of a pretentious name. They insist on making it about sorcery first, leadership second, at Blakeley. But it is true that many prominent leaders are also prominent sorcerors. It is a mark of distinguishment." -> joke_opts
 
 * (joke_alex) {not joke_cren} [You make a joke at Alex.] You smile at Alex. She's gotta manage to graduate first, before she can have that easy life.
 
 "Yeah yeah, I know." Alex rolls her eyes. "It's fine. Cs get degrees. You're not my mom, stop worrying about me." -> joke_opts
 
 * (you_mia_major) [You ask what Mia's majoring in.] You look over at Mia. How about her? What's she thinking about majoring in?
 
 Mia is slow to answer. "Well... I haven't decided for sure. But, yeah. I've been thinking..." She tilts her head up. "Nursing."
 
 Alex blinks. "Just... Nursing?"
 
 Mia blushes, but tilts her head up defiantly. "Yes. It... feels right. I want to help people."
 
 -
 
 * [You think it's right too.] You nod. This is a natural evolution for Mia. It makes a lot of sense.
 
 Alex isn't so sure. "I... If that's what you want, Mia. But you must know that it's a very physically demanding job."
 
 Mia nods. "I know. I've done some research."
 
 * [You think it's kind of a waste of Mia's talent.] Mia's so talented. She would be better off with a hard-sorcery track. Wouldn't that help more people?
 
 Mia shakes her head. "No. I want to <i>help</i> people. Directly. I want to know that I'm having an impact."
 
 - You look at Cren. He looks a little troubled, but does not say anything - only hugs Mia a bit tighter.
 
 Alex sighs. She turns her eyes you. "How 'bout you, {player_name}? What're you thinking you're gonna major in?"
 
 You recall that, at the beginning of the school year, you said you joined Blakeley <>
 
 {player_blakeley_motivation:
 - "find_job": to find a good job.
 - "help_people": to help people.
 - "create_art": to create great art.
 - "develop_technomancy": to develop the technomancy of the future.
 - else: because it simply seemed like the thing to do after secondary school.
 }
 
 In your sophomore year, what do you plan on choosing as your major?
 
 * [INPUT: player_major]
 
 - You're going to major in {player_major}.
 
 {to_lower(player_major):
 - "sorcerous leadership": Alex makes a face. "No way you're being serious. I bet you haven't even thought about it yet."
 - "arcane compuation": Alex tilts her head and half-smiles. "Oh, really? You following me around like a puppy, or what?" She flaps her hand. "Nah, I getcha. The job prospects are too good to ignore."
 - "material artifice": Alex groans. "No way. Good luck with Lundgren. Seriously. You're gonna need it."
 - "nursing": Alex blinks. "No, really? You too?" She tilts her head. "You two really are hopeless."
 - else: "Huh." Alex muses. Then she smiles - a real, full smile. "Sure. I think it fits you."
 }
 
 * [Continue]
 
 - The magelight flickers again. "Oops, looks like {yourself_light: {player_name}|I} mighta done bad job with that spell," Alex says. "Someone else give it a shot?"
 
 Cren and Mia share a look. Cren starts, "Mayhaps -"
 
 Mia interrupts him firmly. "I'll renew it." She takes a pinch of astral dust and makes confident gestures, casting the dust onto the light. It glows brightly and steadily.
 
 Mia sits back. She sighs, glancing at her pouch of astral dust. She shakes her head. Looks up at the stars. The rest of you follow.
 
 * [Continue]
 
 Cren speaks. "Back in Moran... as a kid, my tutor would take me to the top of the tallest tower at the keep, and teach me the constellations. I could name all of them. But now..." He shakes his head. "The stars are completely different. They've all moved."
 
 A blanket of silence follows. You all stare at the stars.
 
 -  You wonder how much the stars have drifted. You wonder about a future where the stars no longer align.
 
 You feel... <>
 
 * [Confident.] You feel confident about the future.
 * [Melancholic.] You feel a deep melancholy when you think about the future.
 * [Excited.] You feel excited for what the future holds.
 * [Anxious.] You feel anxiety over what may come.
 * [Something else.]
 
 As you look up at the stars, and think about the future, you feel...
 
 ** [INPUT: player_final_feeling] You feel {player_final_feeling} about what may come.
 
 * [You're not sure what you feel.] You don't truly know what to feel about the future. Everything is so uncertain.
 
 - Four friends stare at the sky. Wind blows through the trees. Meteors make streaks in the sky. The stars wheel overhead.
 
 * [Time passes, as it ever does, and ever will.]
 
-> DONE




















